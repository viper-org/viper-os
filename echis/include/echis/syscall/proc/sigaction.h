#ifndef VIPEROS_ECHIS_SYSCALL_PROC_SIGACTION_H
#define VIPEROS_ECHIS_SYSCALL_PROC_SIGACTION_H 1

#include <stddef.h>

namespace echis
{
    namespace syscall
    {
        using SignalHandler = void(*)(int);

        int sigaction(int signum, SignalHandler handler);
    }
}

#endif // VIPEROS_ECHIS_SYSCALL_PROC_SIGACTION_H