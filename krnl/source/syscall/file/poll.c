#include "syscall/syscalls.h"

#include "event/bus.h"

#include "sched/pipe.h"
#include "sched/sched.h"

int sys_poll1(int fd)
{
    struct process *proc = sched_curr()->owner;

    if (proc->fds[fd].pipe && proc->fds[fd].pipe->type != PIPE_READ) return -1;

    struct poll_event_object *event = create_poll_event(&proc->fds[fd]);
    proc->fds[fd].enext = NULL;

    if (proc->fds[fd].pipe)
    {
        struct pipe* write = proc->fds[fd].pipe->read.write_end;
        write->poll_event = event;
    }

    wait_on_object(&event->obj, sched_curr());
    sched_blockcurr();

    return 0;
}

int sys_poll(int *fds, size_t nfds)
{
    struct process *proc = sched_curr()->owner;

    struct poll_event_object *event = create_poll_event(&proc->fds[fds[0]]);

    for (size_t i = 0; i < nfds; ++i)
    {
        int fd = fds[i];
        if (proc->fds[fd].pipe && proc->fds[fd].pipe->type != PIPE_READ) return -1;

        if (proc->fds[fd].pipe)
        {
            struct pipe* write = proc->fds[fd].pipe->read.write_end;
            if (write->write.size) // there is data to read
                return fd;
            write->poll_event = event;
        }
        proc->fds[fds[i]].enext = &proc->fds[fds[i+1]];
    }
    proc->fds[fds[nfds-1]].enext = NULL;

    wait_on_object(&event->obj, sched_curr());
    sched_blockcurr();
    
    for (size_t i = 0; i < nfds; ++i)
    {
        if (proc->fds[fds[i]].pipe && event->out->pipe)
        {
            if (proc->fds[fds[i]].pipe->read.write_end == event->out->pipe) return fds[i];
        }
        if (&proc->fds[fds[i]] == event->out) return fds[i];
    }

    return -1;
}
