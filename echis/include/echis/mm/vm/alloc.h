#ifndef VIPEROS_ECHIS_MM_VM_ALLOC_H
#define VIPEROS_ECHIS_MM_VM_ALLOC_H 1

#include <atheris/mm/vm.h>

#include <cstdint>

namespace echis
{
    namespace vm
    {
        namespace alloc
        {
            void Init();

            void* GetPages(atheris::vm::AddressSpace* addressSpace, std::uint32_t pageCount, std::uint16_t flags);
            void* GetKernelPages(std::uint32_t pageCount, std::uint16_t flags);

            // Returns false if the region is already allocated, else returns true
            bool MarkUsed(atheris::vm::AddressSpace* addressSpace, std::uint64_t base, std::uint32_t pageCount);
            bool MarkKernelUsed(std::uint64_t base, std::uint32_t pageCount);
        }
    }
}

#endif // VIPEROS_ECHIS_MM_VM_ALLOC_H