#ifndef VIPEROS_ECHIS_SYSCALL_FILE_READ_H
#define VIPEROS_ECHIS_SYSCALL_FILE_READ_H 1

#include <stddef.h>

namespace echis
{
    namespace syscall
    {
        size_t read(int fd, void* buf, size_t count);
    }
}

#endif // VIPEROS_ECHIS_SYSCALL_FILE_READ_H