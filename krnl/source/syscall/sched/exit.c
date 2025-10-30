#include "syscall/syscalls.h"

#include "sched/proc.h"
#include "sched/sched.h"

#include "driver/dbg.h"

void sys_exit(int code)
{
    (void)code; // todo: pass code to the exit event
    dbg_printf("EXIT called with pid=%d, code=%d", sched_curr()->owner->pid, code);
    thread_kill(sched_curr());
}
