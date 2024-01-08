#ifndef VIPEROS_ECHIS_MM_HEAP_H
#define VIPEROS_ECHIS_MM_HEAP_H 1

#include <stddef.h>

namespace echis
{
    namespace mm
    {
        /*
         * Initializes the heap
         */
        void Init();

        /*
         * Allocates at least count
         * bytes from the heap
         */
        void* Alloc(size_t count);

        /*
         * Frees a previously allocated
         * memory region
         */
        void Free(void* mem);
    }
}

#endif // VIPEROS_ECHIS_MM_HEAP_H