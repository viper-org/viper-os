#include "event/object.h"
#include "syscall/syscalls.h"

#include "event/bus.h"

#include "driver/dbg.h"

#include "sched/sched.h"

int sys_waitpid(int pid, int *status, int options)
{
    (void)options;
    struct thread *wait_on = &find_proc(pid)->main_thread;
    struct exit_event_object *e;
    if (wait_on->exit_event) e = wait_on->exit_event;
    else e = create_exit_event(wait_on);
    
    wait_on_object(&e->obj, sched_curr());
    dbg_printf("WAIT called with pid=%d, caller_pid=%d", pid, sched_curr()->owner->pid);
    sched_blockcurr();
    *status = e->exit_code; // todo: get exit code from event
    return pid;
}
