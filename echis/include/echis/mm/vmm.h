#ifndef VIPEROS_ECHIS_MM_VMM_H
#define VIPEROS_ECHIS_MM_VMM_H 1

#include <atheris/mm/vm.h>

namespace echis
{
    namespace vm
    {
        void* GetPages (atheris::vm::AddressSpace* addressSpace, uint64_t count, uint16_t flags);
        void  FreePages(atheris::vm::AddressSpace* addressSpace, void* pages, uint64_t count);
    }
}

#endif // VIPEROS_ECHIS_MM_VMM_H