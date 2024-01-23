#include <atheris/private/mm/vm.h>

#include <mm/util.h>

#include <cpu/asm.h>

#include <echis/mm/pmm.h>

#include <limine.h>

#include <string.h>

namespace atheris
{
    using namespace x64;
    namespace pm
    {
        extern limine_memmap_response* memoryMap;
    }

    namespace vm
    {
        extern "C" char _kernel_start[];
        extern "C" char _kernel_end[];

        AddressSpace kernelAddressSpace;
        AddressSpace* currentAddressSpace = nullptr;

        static volatile limine_kernel_address_request kernelAddressRequest = {
            .id = LIMINE_KERNEL_ADDRESS_REQUEST,
            .revision = 1,
            .response = nullptr
        };

        AddressSpace AddressSpace::Create()
        {
            AddressSpace ret;
            echis::pmm::physaddr newPml4 = echis::pmm::GetPage();
            memset(reinterpret_cast<void*>(PhysToVirt(newPml4)), 0, PageSize);
            ret.pml4 = ValidatePageTable(newPml4);

            void* higherHalf       = reinterpret_cast<void*>(PhysToVirt(ret.pml4 + (256 * sizeof(uint64_t))));
            void* kernelHigherHalf = reinterpret_cast<void*>(PhysToVirt(kernelAddressSpace.pml4 + (256 * sizeof(uint64_t))));

            memcpy(higherHalf, kernelHigherHalf, 256 * sizeof(uint64_t));

            ret.freeList = new VMNode;
            ret.freeList->base  = 0x1000;
            ret.freeList->pages = NPAGES(0x00007FFFFFFFE000 - 0x1000); // Ignore top and bottom pages
            ret.freeList->next  = nullptr;

            return ret;
        }

        void AddressSpace::switchTo()
        {
            currentAddressSpace = this;
            x64::cpu::WriteCR<3>(pml4);
        }

        AddressSpace* AddressSpace::Current()
        {
            return currentAddressSpace;
        }

        void CreateKernelPML4()
        {
            echis::pmm::physaddr pml4 = echis::pmm::GetPage();
            memset(reinterpret_cast<void*>(PhysToVirt(pml4)), 0, PageSize);
            kernelAddressSpace.pml4 = ValidatePageTable(pml4);
        }

        void CreateGlobalPoolPages()
        {
            for(int i = 256; i < 512; i++)
            {
                uint64_t* pml4 = reinterpret_cast<uint64_t*>(kernelAddressSpace.pml4);
                pml4[i] = echis::pmm::GetPage();
                memset(reinterpret_cast<uint8_t*>(PhysToVirt(ValidatePageTable(pml4[i]))), 0, PageSize);
                pml4[i] |= 0x3;
            }
        }

        void MapKernel()
        {
            echis::pmm::physaddr physicalBase = kernelAddressRequest.response->physical_base;
            echis::pmm::physaddr virtualBase  = kernelAddressRequest.response->virtual_base;
            
            MapPages(&kernelAddressSpace, physicalBase, virtualBase, flags::present | flags::write, NPAGES(_kernel_end - _kernel_start));
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

            CreateGlobalPoolPages();
            MapKernel();
            MapHHDM();
        }

        void Init()
        {
            CreateKernelPageTables();

            kernelAddressSpace.switchTo();

            currentAddressSpace = &kernelAddressSpace;
        }

        void APInstallKernelPageTables()
        {
            kernelAddressSpace.switchTo();
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

        void* GetVirtualAddress(echis::pmm::physaddr physaddr)
        {
            return reinterpret_cast<void*>(PhysToVirt(physaddr));
        }

        uint16_t GetFlags(AddressSpace* addressSpace, uint64_t virtualAddress)
        {
            if (addressSpace == nullptr)
            {
                addressSpace = currentAddressSpace;
            }

            virtualAddress  &= ~0xfff;

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

                if (!(pt[index] & flags::present) && !(pt[index] & flags::lazy))
                {
                   return 0;
                }

                pt = reinterpret_cast<uint64_t*>(PhysToVirt(ValidatePageTable(pt[index])));
            }

            return pt[index] & 0xfff;
        }

        bool HandlePageFault(x64::cpu::Context* context)
        {
            if (auto faultingAddress = context->ControlRegisters.cr2)
            {
                uint16_t flags = GetFlags(nullptr, faultingAddress);
                if (flags & flags::lazy)
                {
                    flags &= ~flags::lazy;
                    flags |=  flags::present;
                    MapPage(nullptr, echis::pmm::GetPage(), faultingAddress, flags);
                    context->ControlRegisters.cr2 = 0;
                    return false;
                }
            }
            return true;
        }
    }
}