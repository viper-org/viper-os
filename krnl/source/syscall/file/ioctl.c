#include "sched/sched.h"
#include "syscall/syscalls.h"

int sys_ioctl(int fd, unsigned long op, char *argp)
{
    struct process *proc = sched_curr()->owner;
    struct vnode *node = proc->fds[fd].vnode;
    if (!node) return -1;
    node->fs->ioctl(node, op, argp);
    return 0;
}
