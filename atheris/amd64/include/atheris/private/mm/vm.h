#ifndef VIPEROS_ATHERIS_AMD64_ATHERIS_PRIVATE_MM_VM_H
#define VIPEROS_ATHERIS_AMD64_ATHERIS_PRIVATE_MM_VM_H 1

#include <atheris/mm/pm.h>

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

            void switchTo();
        };

        void* GetVirtualAddress(pm::physaddr physAddress);

        
        void Init();

        void MapPage(AddressSpace* addressSpace, pm::physaddr phys, uint64_t virt, uint16_t flags);
        void MapPages(AddressSpace* addressSpace, pm::physaddr phys, uint64_t virt, uint16_t flags, uint32_t pageCount);
    }
}

#endif // VIPEROS_ATHERIS_AMD64_ATHERIS_PRIVATE_MM_VM_H