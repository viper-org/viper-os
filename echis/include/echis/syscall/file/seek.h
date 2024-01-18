#ifndef VIPEROS_ECHIS_SYSCALL_FILE_SEEK_H
#define VIPEROS_ECHIS_SYSCALL_FILE_SEEK_H 1

#include <stddef.h>

namespace echis
{
    namespace syscall
    {
        size_t seek(int fd, size_t offset);
    }
}

#endif // VIPEROS_ECHIS_SYSCALL_FILE_SEEK_H