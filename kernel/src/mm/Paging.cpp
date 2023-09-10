#include <mm/Paging.h>
#include <mm/Pmm.h>

#include <string.h>

#include <limine.h>

#include <stddef.h>

namespace PMM
{
    extern limine_memmap_response* MemMap;
}

static volatile limine_kernel_address_request kernelAddressRequest = {
    .id = LIMINE_KERNEL_ADDRESS_REQUEST,
    .revision = 1,
    .response = nullptr
};

extern char _kernel_start[];
extern char _kernel_end[];

namespace Paging
{
    AddressSpace kernelAddrSpace;

    #define PRESENT_WRITABLE 0x3

    static inline uint64_t Entry(uint64_t pt)
    {
        return (pt & ~0xFFFF000000000FFF);
    }

    void Init()
    {
        kernelAddrSpace.pml4 = (uint64_t*)PMM::GetPage();
        memset((uint8_t*)PMM::PhysToVirt((uint64_t)kernelAddrSpace.pml4), 0, PMM::PAGE_SIZE);
        kernelAddrSpace.pml4 = (uint64_t*)Entry((uint64_t)kernelAddrSpace.pml4);

        MapPages(&kernelAddrSpace, 
                  (uint64_t)kernelAddressRequest.response->physical_base, (uint64_t)kernelAddressRequest.response->virtual_base,
                  PRESENT_WRITABLE, PMM::NPAGES(_kernel_end - _kernel_start));
        MapPages(&kernelAddrSpace, 0, 0xFFFF800000000000, 3, PMM::NPAGES(0x40000000));
        
        for(size_t i = 0; i < PMM::MemMap->entry_count; i++)
        {
            limine_memmap_entry* entry = PMM::MemMap->entries[i];

            if(entry->base + entry->length < 0x400000)
                continue;

            MapPages(&kernelAddrSpace, entry->base, entry->base + 0xFFFF800000000000, PRESENT_WRITABLE, PMM::NPAGES(entry->length));
        }
 
        asm volatile("mov %0, %%rax; mov %%rax, %%cr3" : : "m"(kernelAddrSpace.pml4));
    }

    void MapPage(AddressSpace* addrspace, uint64_t PhysAddr, uint64_t VirtAddr, uint16_t Flags)
    {
        if(addrspace == nullptr)
            addrspace = &kernelAddrSpace;
        PhysAddr &= ~0xFFF;
        VirtAddr &= ~0xFFF;
        Flags    &=  0xFFF;

        if(VirtAddr & ((uint64_t)1 << 47))
        {
            if((VirtAddr & 0xFFFF000000000000) != 0xFFFF000000000000) // TODO: Panic
                asm volatile("cli; hlt");
        }

        uint64_t shift = 48;
        uint64_t* pt = (uint64_t*)PMM::PhysToVirt(Entry((uint64_t)addrspace->pml4));
        uint64_t idx;
        for(char i = 0; i < 4; i++)
        {
            shift -= 9;
            idx = (VirtAddr >> shift) & 0x1FF;
            if(i == 3)
                break;
            
            if(!(pt[idx] & 0x1))
            {
                pt[idx] = (uint64_t)PMM::GetPage();
                memset((uint8_t*)PMM::PhysToVirt(Entry(pt[idx])), 0, PMM::PAGE_SIZE);
                pt[idx] |= Flags | 1;
            }
            pt = (uint64_t*)PMM::PhysToVirt(Entry(pt[idx]));
        }
        pt[idx] = PhysAddr | Flags;
    }

    void MapPages(AddressSpace* addrspace, uint64_t PhysAddr, uint64_t VirtAddr, uint16_t Flags, uint64_t Npages)
    {
        for(uint64_t i = 0; i < Npages * PMM::PAGE_SIZE; i += PMM::PAGE_SIZE)
            MapPage(addrspace, PhysAddr + i, VirtAddr + i, Flags);
    }

    AddressSpace* KernelAddrSpace()
    {
        return &kernelAddrSpace;
    }
}