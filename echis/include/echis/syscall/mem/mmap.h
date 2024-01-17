#ifndef VIPEROS_ECHIS_SYSCALL_MEM_MMAP_H
#define VIPEROS_ECHIS_SYSCALL_MEM_MMAP_H 1

#include <stddef.h>

namespace echis
{
    namespace syscall
    {
        void* mmap(size_t length); // TODO: Add proper functionality
    }
}

#endif // VIPEROS_ECHIS_SYSCALL_MEM_MMAP_H