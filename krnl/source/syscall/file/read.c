#include "syscall/syscalls.h"

#include "sched/proc.h"
#include "sched/sched.h"
#include "sched/pipe.h"

#include "driver/dbg.h"

ssize_t sys_read(int fd, void *buf, size_t count)
{
    // todo: verify buf
    struct process *proc = sched_curr()->owner;
    struct proc_fd *desc = &proc->fds[fd];

    dbg_printf("READ called with fd=%d,count=%d\n", fd, count);

    if (desc->pipe)
    {
        if (desc->pipe->type != PIPE_READ) return -1;
        count = read_pipe(desc->pipe, buf, count);
    }
    else
    {
        struct vnode *node = desc->vnode;

        node->fs->read(node, buf, &count, desc->seek);
        desc->seek += count;
    }

    dbg_printf("Actually read %d bytes\n\n", count);
    
    return count;
}
