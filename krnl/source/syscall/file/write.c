#include "syscall/syscalls.h"

#include "sched/proc.h"
#include "sched/sched.h"

#include "driver/dbg.h"

ssize_t sys_write(int fd, const void *buf, size_t count)
{
    // todo: verify buf
    struct process *proc = sched_curr()->owner;
    struct proc_fd *desc = &proc->fds[fd];
    struct vnode *node = desc->vnode;

    dbg_printf("WRITE called with fd=%d,count=%d\n\n", fd, count);

    node->fs->write(node, buf, count, desc->seek);
    desc->seek += count;
    
    return count;
}
