#include "syscall/syscalls.h"

#include "sched/sched.h"

#include "driver/dbg.h"

void sys_yield(void)
{
    struct process *proc = sched_curr()->owner;
    dbg_printf("YIELD called with pid=%d\n\n", proc->pid);
    sched_yield();
}
