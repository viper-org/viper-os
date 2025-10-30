#include "sched/sched.h"
#include "syscall/syscalls.h"

#include "fs/vfs.h"

int sys_stat(const char *path, struct stat *statbuf)
{
    // todo: verify pointers
    struct vnode *node = lookuppn(path);
    return node->fs->stat(node, statbuf);
}

int sys_fstat(int fd, struct stat *statbuf)
{
    // todo: verify pointers
    struct process *proc = sched_curr()->owner;
    struct vnode *node = proc->fds[fd].vnode;
    return node->fs->stat(node, statbuf);
}
