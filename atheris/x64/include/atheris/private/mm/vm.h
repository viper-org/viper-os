#ifndef VIPEROS_ATHERIS_X64_ATHERIS_MM_VM_H
#define VIPEROS_ATHERIS_X64_ATHERIS_MM_VM_H 1

#include <echis/mm/physaddr.h>

#include <stdint.h>

namespace atheris
{
    namespace vm
    {
        struct AddressSpace
        {

            void switchTo();
            
            echis::pmm::physaddr pml4;

            static AddressSpace* Current();
            static AddressSpace Create();
        };

        void Init();
        void APInstallKernelPageTables();

        void MapPage (AddressSpace* addressSpace, echis::pmm::physaddr physicalAddress, uint64_t virtualAddress, uint16_t flags);
        void MapPages(AddressSpace* addressSpace, echis::pmm::physaddr physicalAddress, uint64_t virtualAddress, uint16_t flags, uint32_t pageCount);

        void* GetVirtualAddress(echis::pmm::physaddr physaddr);

        namespace flags
        {
            enum flags : uint16_t
            {
                present = 1 << 0,
                write   = 1 << 1,
                user    = 1 << 2,
            };
        }
    }
}

#endif // VIPEROS_ATHERIS_X64_ATHERIS_MM_VM_H