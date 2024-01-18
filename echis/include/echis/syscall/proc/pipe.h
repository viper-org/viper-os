#ifndef VIPEROS_ECHIS_SYSCALL_PROC_PIPE_H
#define VIPEROS_ECHIS_SYSCALL_PROC_PIPE_H 1

#include <stddef.h>

namespace echis
{
    namespace syscall
    {
        int pipe(int fds[2]);
    }
}

#endif // VIPEROS_ECHIS_SYSCALL_PROC_PIPE_H