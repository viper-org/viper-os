#include "syscall/syscalls.h"

#include "sched/sched.h"

int sys_getpid(void)
{
    return sched_curr()->owner->pid;
}
