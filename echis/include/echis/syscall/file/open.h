#ifndef VIPEROS_ECHIS_SYSCALL_FILE_OPEN_H
#define VIPEROS_ECHIS_SYSCALL_FILE_OPEN_H 1

#include <stddef.h>

namespace echis
{
    namespace syscall
    {
        int open(const char* path, int flags);
    }
}

#endif // VIPEROS_ECHIS_SYSCALL_FILE_OPEN_H