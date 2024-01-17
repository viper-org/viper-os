#ifndef VIPEROS_ECHIS_SYSCALL_FILE_CLOSE_H
#define VIPEROS_ECHIS_SYSCALL_FILE_CLOSE_H 1

#include <stddef.h>

namespace echis
{
    namespace syscall
    {
        int close(int fd);
    }
}

#endif // VIPEROS_ECHIS_SYSCALL_FILE_CLOSE_H