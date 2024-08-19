#ifndef VIPEROS_ATHERIS_AMD64_ATHERIS_PRIVATE_MM_VM_H
#define VIPEROS_ATHERIS_AMD64_ATHERIS_PRIVATE_MM_VM_H 1

#include <atheris/mm/pm.h>

#include <echis/mm/vm/node.h>

#include <list>

namespace atheris
{
    namespace vm
    {
        namespace flags
        {
            enum flags : uint16_t
            {
                present = (1 << 0),
                write   = (1 << 1),
            };
        }

        struct AddressSpace
        {
            pm::physaddr pml4;
            std::list<echis::vm::VMAllocNode> nodes;

            void switchTo();

            static AddressSpace Create();
        };

        void* GetVirtualAddress(pm::physaddr physAddress);

        
        void Init();
        void InitAllocator(std::list<echis::vm::VMAllocNode>& freeList);

        void MapPage(AddressSpace* addressSpace, pm::physaddr phys, uint64_t virt, uint16_t flags);
        void MapPages(AddressSpace* addressSpace, pm::physaddr phys, uint64_t virt, uint16_t flags, uint32_t pageCount);
    }
}

#endif // VIPEROS_ATHERIS_AMD64_ATHERIS_PRIVATE_MM_VM_H