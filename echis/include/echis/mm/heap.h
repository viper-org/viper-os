#ifndef VIPEROS_ECHIS_MM_HEAP_H
#define VIPEROS_ECHIS_MM_HEAP_H 1

#include <cstddef>

namespace echis
{
    namespace mm
    {
        void Init();
        /*
         * Tell the VMM about the pages the heap allocator mapped
         * so it doesn't try to allocate them
         */
        void MarkMemUsed();

        void* AllocateMemory(std::size_t count);

        void FreeMemory(void* mem);
    }
}

#endif // VIPEROS_ECHIS_MM_HEAP_H