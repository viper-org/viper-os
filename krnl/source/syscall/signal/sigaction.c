#include "syscall/syscalls.h"

#include "sched/sched.h"

int sys_sigaction(int signum, void (*handler)(int))
{
    sched_curr()->signal_handlers[signum] = handler;
    return 0;
}
