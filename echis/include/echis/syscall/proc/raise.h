#ifndef VIPEROS_ECHIS_SYSCALL_PROC_RAISE_H
#define VIPEROS_ECHIS_SYSCALL_PROC_RAISE_H 1

#include <stddef.h>

namespace echis
{
    namespace syscall
    {
        int raise(int sig);
    }
}

#endif // VIPEROS_ECHIS_SYSCALL_PROC_RAISE_H