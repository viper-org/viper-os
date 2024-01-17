#ifndef VIPEROS_ECHIS_SYSCALL_FILE_WRITE_H
#define VIPEROS_ECHIS_SYSCALL_FILE_WRITE_H 1

#include <stddef.h>

namespace echis
{
    namespace syscall
    {
        size_t write(int fd, const void* buf, size_t count);
    }
}

#endif // VIPEROS_ECHIS_SYSCALL_FILE_WRITE_H