#ifndef VIPEROS_ECHIS_SYSCALL_FILE_IOCTL_H
#define VIPEROS_ECHIS_SYSCALL_FILE_IOCTL_H 1

#include <stddef.h>

namespace echis
{
    namespace syscall
    {
        int ioctl(int fd, unsigned long request, char* arg);
    }
}

#endif // VIPEROS_ECHIS_SYSCALL_FILE_IOCTL_H