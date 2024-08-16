#include <atheris/mm/vm.h>

#include <mm/pm.h>

#include <util/debug.h>

#include <echis/mm/physical.h>

#include <echis/driver/debugcon.h>

#include <limine.h>

#include <lazy_init.h>
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

        lazy_init<AddressSpace> kernelAddressSpace;

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

        static std::uint64_t hhdmTop = 0;

        static inline void CreateKernelPML4()
        {
            kernelAddressSpace = AddressSpace();
            kernelAddressSpace->pml4 = GetPageLevel();
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
            hhdmTop = hhdmBase + 0x400000;

            // Map all memory map entries
            for (std::uint64_t i = 0; i < amd64::pm::GetMemoryMap()->entry_count; ++i)
            {
                auto entry = amd64::pm::GetMemoryMap()->entries[i];

                MapPages(&kernelAddressSpace,
                    entry->base,
                    reinterpret_cast<uint64_t>(GetVirtualAddress(entry->base)),
                    flags::present | flags::write,
                    NPAGES(entry->length));
                
                std::uint64_t entryTop = reinterpret_cast<uint64_t>(GetVirtualAddress(entry->base)) + entry->length;
                if (entryTop > hhdmTop)
                {
                    hhdmTop = entryTop;
                }
            }
        }

        void Init()
        {
            CreateKernelPML4();

            MapKernel();
            MapHHDM();

            kernelAddressSpace->switchTo();
        }

        void InitAllocator(std::list<echis::vm::VMAllocNode>& freeList)
        {
            echis::vm::VMAllocNode lowerNode;
            lowerNode.base = hhdmTop;
            auto size = (std::uint64_t)_kernel_start - hhdmTop;
            lowerNode.numPages = NPAGES(size);
            freeList.push_back(lowerNode);

            echis::vm::VMAllocNode higherNode;
            higherNode.base = (std::uint64_t)_kernel_end;
            size = 0xffffffffffffe000 - (std::uint64_t)_kernel_end;
            higherNode.numPages = NPAGES(size);
            freeList.push_back(higherNode);
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


        AddressSpace AddressSpace::Create()
        {
            AddressSpace ret;
            ret.pml4 = echis::mm::physical::GetPage();

            void* higherHalf       = reinterpret_cast<void*>(GetVirtualAddress(ret.pml4 + (256 * sizeof(uint64_t))));
            void* kernelHigherHalf = reinterpret_cast<void*>(GetVirtualAddress(kernelAddressSpace->pml4 + (256 * sizeof(uint64_t))));

            std::memcpy(higherHalf, kernelHigherHalf, 256 * sizeof(uint64_t));

            echis::vm::VMAllocNode node;
            node.base = 0x1000; // skip null page
            node.numPages = NPAGES(0x00007FFFFFFFE000ull - 0x1000ull);
            ret.nodes.push_back(node);

            return ret;
        }
    }
}