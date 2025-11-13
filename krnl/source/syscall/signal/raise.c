#include "syscall/syscalls.h"

#include "signal/signal.h"

int sys_raise(int sig)
{
    if (sig < 0 || sig > 32) return 1;

    deliver_to_current(sig);
    return 0;
}
