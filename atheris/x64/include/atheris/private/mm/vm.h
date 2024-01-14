#ifndef VIPEROS_ATHERIS_X64_ATHERIS_MM_VM_H
#define VIPEROS_ATHERIS_X64_ATHERIS_MM_VM_H 1

#include <cpu/context.h>

#include <echis/mm/physaddr.h>

#include <stdint.h>
#include <stddef.h>

namespace atheris
{
    namespace vm
    {
        struct VMNode
        {
            size_t   pages;
            uint64_t base;
            VMNode*  next;
        };

        struct AddressSpace
        {
            void switchTo();
            
            echis::pmm::physaddr pml4;
            VMNode* freeList;

            static AddressSpace* Current();
            static AddressSpace Create();
        };

        void Init();
        void APInstallKernelPageTables();

        bool HandlePageFault(x64::cpu::Context* context);

        void MapPage (AddressSpace* addressSpace, echis::pmm::physaddr physicalAddress, uint64_t virtualAddress, uint16_t flags);
        void MapPages(AddressSpace* addressSpace, echis::pmm::physaddr physicalAddress, uint64_t virtualAddress, uint16_t flags, uint32_t pageCount);

        void* GetVirtualAddress(echis::pmm::physaddr physaddr);

        uint16_t GetFlags(AddressSpace* addressSpace, uint64_t virtualAddress);

        namespace flags
        {
            enum flags : uint16_t
            {
                present = 1 << 0,
                write   = 1 << 1,
                user    = 1 << 2,
                lazy    = 1 << 10,
            };
        }
    }
}

#endif // VIPEROS_ATHERIS_X64_ATHERIS_MM_VM_H