#include <atheris/mm/vm.h>

#include <mm/pm.h>

#include <util/debug.h>

#include <echis/mm/physical.h>

#include <echis/driver/debugcon.h>

#include <limine.h>

#include <cstring>

static volatile limine_hhdm_request hhdmRequest = {
    .id = LIMINE_HHDM_REQUEST,
    .revision = 1,
    .response = nullptr
};

static volatile limine_kernel_address_request kernelAddressRequest = {
    .id = LIMINE_KERNEL_ADDRESS_REQUEST,
    .revision = 1,
    .response = nullptr
};

extern "C" char _kernel_start[];
extern "C" char _kernel_end[];

#define NPAGES(sz) ((sz + 0x1000 - 1) / 0x1000)

namespace atheris
{
    namespace vm
    {
        constexpr unsigned int PageSize = 0x1000;

        AddressSpace kernelAddressSpace;

        void AddressSpace::switchTo()
        {
            asm volatile("mov %0, %%cr3" :: "r"(pml4));
        }

        static inline pm::physaddr GetPageTableAddress(pm::physaddr pt)
        {
            return (pt & ~0xFFFF000000000FFF);
        }

        static inline pm::physaddr GetPageLevel()
        {
            pm::physaddr phys = echis::mm::physical::GetPage();
            void* virt = GetVirtualAddress(GetPageTableAddress(phys));
            std::memset(virt, 0, PageSize);
            return phys;
        }


        static inline void CreateKernelPML4()
        {
            kernelAddressSpace.pml4 = GetPageLevel();
        }

        static inline void MapKernel()
        {
            pm::physaddr physicalBase = kernelAddressRequest.response->physical_base;
            std::uint64_t virtualBase = kernelAddressRequest.response->virtual_base;

            MapPages(&kernelAddressSpace,
                physicalBase,
                virtualBase,
                flags::present | flags::write,
                NPAGES(_kernel_end - _kernel_start));
        }

        static inline void MapHHDM()
        {
            uint64_t hhdmBase = hhdmRequest.response->offset;

            // Map lower 4G
            MapPages(&kernelAddressSpace,
                0,
                hhdmBase,
                flags::present | flags::write,
                NPAGES(0x400000));

            // Map all memory map entries
            for (std::uint64_t i = 0; i < amd64::pm::GetMemoryMap()->entry_count; ++i)
            {
                auto entry = amd64::pm::GetMemoryMap()->entries[i];

                MapPages(&kernelAddressSpace,
                    entry->base,
                    reinterpret_cast<uint64_t>(GetVirtualAddress(entry->base)),
                    flags::present | flags::write,
                    NPAGES(entry->length));
            }
        }

        void Init()
        {
            CreateKernelPML4();

            MapKernel();
            MapHHDM();

            kernelAddressSpace.switchTo();
        }

        void MapPage(AddressSpace* addressSpace, pm::physaddr phys, uint64_t virt, uint16_t flags)
        {
            if (!addressSpace)
            {
                addressSpace = &kernelAddressSpace;
            }

            phys  &= ~0xFFF;
            virt  &= ~0xFFF;
            flags &=  0xFFF;

            std::uint64_t shift = 48;
            std::uint64_t* pageTable = reinterpret_cast<std::uint64_t*>(GetVirtualAddress(GetPageTableAddress(addressSpace->pml4)));
            std::uint64_t index;
            for (int i = 0; i < 4; ++i)
            {
                shift -= 9;
                index = (virt >> shift) & 0x1FF;
                if (i == 3) break;

                if (!(pageTable[index] & flags::present))
                {
                    pageTable[index] = GetPageLevel();
                    pageTable[index] |= flags | flags::present;
                }

                pageTable = reinterpret_cast<std::uint64_t*>(GetVirtualAddress(GetPageTableAddress(pageTable[index])));
            }

            pageTable[index] = phys | flags;
        }

        void MapPages(AddressSpace* addressSpace, pm::physaddr phys, uint64_t virt, uint16_t flags, uint32_t pageCount)
        {
#ifdef ATHERIS_VM_LOG_MAPPINGS
            echis::driver::debugcon::WriteFormatted("[VMM] Mapping %d pages from %p to %p with flags %d\n", pageCount, phys, virt, flags);
#endif

            for (std::uint64_t i = 0; i < pageCount * PageSize; i += PageSize)
            {
                MapPage(addressSpace,
                    phys + i,
                    virt + i,
                    flags);
            }
        }


        void* GetVirtualAddress(pm::physaddr physAddress)
        {
            // maybe check that response is valid here
            return reinterpret_cast<void*>(hhdmRequest.response->offset + physAddress);
        }
    }
}