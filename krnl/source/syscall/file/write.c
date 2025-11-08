#include "event/bus.h"
#include "syscall/syscalls.h"

#include "sched/proc.h"
#include "sched/sched.h"
#include "sched/pipe.h"

#include "event/object.h"

ssize_t sys_write(int fd, const void *buf, size_t count)
{
    // todo: verify buf
    struct process *proc = sched_curr()->owner;
    struct proc_fd *desc = &proc->fds[fd];

    if (desc->pipe)
    {
        if (desc->pipe->type != PIPE_WRITE) return -1;

        count = write_pipe(desc->pipe, buf, count);
    }
    else
    {
        struct vnode *node = desc->vnode;
        size_t seek = desc->seek;
        if (desc->flags & O_APPEND)
        {
            struct stat b;
            node->fs->stat(node, &b);
            seek = b.st_size;
        }
        node->fs->write(node, buf, count, seek);
        if (!(desc->flags & O_APPEND)) desc->seek += count;
    }

    if (desc->pipe && desc->pipe->poll_event)
    {
        desc->pipe->poll_event->out = desc;
        ready_event(&desc->pipe->poll_event->obj);
        // todo: free event
    }
    struct poll_event_object *event;
    if ((event = find_poll_event(desc)))
    {
        event->out = desc;
        ready_event(&event->obj);
        // todo: free event
    }
    
    return count;
}
