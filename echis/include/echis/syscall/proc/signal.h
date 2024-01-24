#ifndef VIPEROS_ECHIS_SYSCALL_PROC_SIGNAL_H
#define VIPEROS_ECHIS_SYSCALL_PROC_SIGNAL_H 1

#include <stddef.h>

namespace echis
{
    namespace syscall
    {
        using SignalHandler = void(*)(int);

        SignalHandler signal(int signum, SignalHandler handler);
    }
}

#endif // VIPEROS_ECHIS_SYSCALL_PROC_SIGNAL_H