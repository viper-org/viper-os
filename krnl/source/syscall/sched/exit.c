#include "syscall/syscalls.h"

#include "sched/proc.h"
#include "sched/sched.h"

#include "driver/dbg.h"

void sys_exit(int code)
{
    dbg_printf("EXIT called with pid=%d, code=%d\n\n", sched_curr()->owner->pid, code);
    thread_kill(sched_curr(), code);
}
