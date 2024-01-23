#ifndef VIPEROS_ECHIS_SYSCALL_PROC_WAIT_H
#define VIPEROS_ECHIS_SYSCALL_PROC_WAIT_H 1

#include <stddef.h>

namespace echis
{
    namespace syscall
    {
        int wait(int pid);
    }
}

#endif // VIPEROS_ECHIS_SYSCALL_PROC_WAIT_H