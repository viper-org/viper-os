#ifndef VIPEROS_ECHIS_SYSCALL_FILE_POLL_H
#define VIPEROS_ECHIS_SYSCALL_FILE_POLL_H 1

#include <stddef.h>

namespace echis
{
    namespace syscall
    {
        int poll(int fd);
    }
}

#endif // VIPEROS_ECHIS_SYSCALL_FILE_POLL_H