#include "syscall/syscalls.h"

#include "event/bus.h"

#include "sched/pipe.h"
#include "sched/sched.h"

int sys_poll1(int fd)
{
    struct process *proc = sched_curr()->owner;

    if (proc->fds[fd].pipe && proc->fds[fd].pipe->type != PIPE_READ) return -1;

    struct poll_event_object *event = create_poll_event(&proc->fds[fd]);

    if (proc->fds[fd].pipe)
    {
        struct pipe* write = proc->fds[fd].pipe->read.write_end;
        write->poll_event = event;
    }

    wait_on_object(&event->obj, sched_curr());
    sched_blockcurr();

    return 0;
}
