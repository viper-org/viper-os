#include <atheris/private/mm/vm.h>

#include <mm/util.h>

#include <cpu/asm.h>

#include <echis/mm/pmm.h>

#include <limine.h>

#include <string.h>

namespace atheris
{
    namespace pm
    {
        extern limine_memmap_response* memoryMap;
    }

    namespace vm
    {
        using namespace x64;

        extern "C" char _kernel_start[];
        extern "C" char _kernel_end[];


        AddressSpace kernelAddressSpace;
        AddressSpace* currentAddressSpace = &kernelAddressSpace;

        static volatile limine_kernel_address_request kernelAddressRequest = {
            .id = LIMINE_KERNEL_ADDRESS_REQUEST,
            .revision = 1,
            .response = nullptr
        };

        AddressSpace* AddressSpace::Current()
        {
            return currentAddressSpace;
        }

        void CreateKernelPML4()
        {
            echis::pmm::physaddr pml4 = echis::pmm::GetPage();
            memset(reinterpret_cast<void*>(PhysToVirt(kernelAddressSpace.pml4)), 0, PageSize);
            kernelAddressSpace.pml4 = ValidatePageTable(pml4);
        }

        void MapKernel()
        {
            echis::pmm::physaddr physicalBase = kernelAddressRequest.response->physical_base;
            echis::pmm::physaddr virtualBase  = kernelAddressRequest.response->virtual_base;
            
            MapPages(&kernelAddressSpace, physicalBase, virtualBase, flags::present | flags::write, _kernel_end - _kernel_start);
        }

        void MapHHDM()
        {
            MapPages(&kernelAddressSpace, 0, GetHHDMOffset(), flags::present | flags::write, NPAGES(0x400000));

            for (uint64_t i = 0; i < pm::memoryMap->entry_count; ++i)
            {
                limine_memmap_entry* entry = pm::memoryMap->entries[i];

                MapPages(&kernelAddressSpace, entry->base, PhysToVirt(entry->base), flags::present | flags::write, NPAGES(entry->length));
            }
        }

        void CreateKernelPageTables()
        {
            CreateKernelPML4();

            MapKernel();
            MapHHDM();
        }

        void Init()
        {
            CreateKernelPageTables();

            cpu::WriteCR<3>(kernelAddressSpace.pml4);
        }

        void MapPage(AddressSpace* addressSpace, echis::pmm::physaddr physicalAddress, uint64_t virtualAddress, uint16_t flags)
        {
            if (addressSpace == nullptr)
            {
                addressSpace = currentAddressSpace;
            }

            physicalAddress &= ~0xfff;
            virtualAddress  &= ~0xfff;
            flags           &=  0xfff;

            uint64_t shift = 48;
            uint64_t* pt = reinterpret_cast<uint64_t*>(PhysToVirt(ValidatePageTable(addressSpace->pml4)));
            uint64_t index;
            for (int i = 0; i < 4; ++i)
            {
                shift -= 9;
                index = (virtualAddress >> shift) & 0x1ff;
                if (i == 3)
                {
                    break;
                }

                if (!(pt[index] & flags::present))
                {
                    pt[index] = echis::pmm::GetPage();
                    memset(reinterpret_cast<void*>(PhysToVirt(ValidatePageTable(pt[index]))), 0, PageSize);
                    pt[index] |= flags | flags::present;
                }

                pt = reinterpret_cast<uint64_t*>(PhysToVirt(ValidatePageTable(pt[index])));
            }

            pt[index] = physicalAddress | flags;
        }

        void MapPages(AddressSpace* addressSpace, echis::pmm::physaddr physicalAddress, uint64_t virtualAddress, uint16_t flags, uint32_t pageCount)
        {
            for (uint32_t i = 0; i < pageCount * PageSize; i += PageSize)
            {
                MapPage(addressSpace, physicalAddress + i, virtualAddress + i, flags);
            }
        }
    }
}