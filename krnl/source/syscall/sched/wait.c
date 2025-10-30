#include "event/object.h"
#include "syscall/syscalls.h"

#include "event/bus.h"

#include "driver/dbg.h"

#include "sched/sched.h"

int sys_waitpid(int pid, int *status, int options)
{
    (void)options;
    struct exit_event_object *e = create_exit_event(&find_proc(pid)->main_thread);
    wait_on_object(&e->obj, sched_curr());
    dbg_printf("WAIT called with pid=%d, caller_pid=%d", pid, sched_curr()->owner->pid);
    sched_blockcurr();
    *status = -1; // todo: get exit code from event
    return pid;
}
