#include "syscall/syscalls.h"

#include "sched/sched.h"
#include "sched/proc.h"

#include <string.h>

int sys_dup(int fd)
{
    struct process *proc = sched_curr()->owner;
    int newfd = proc_get_freefd(proc);
    if (newfd == -1) return -1;

    memcpy(&proc->fds[newfd], &proc->fds[fd], sizeof(proc->fds[newfd]));
    return newfd;
}

int sys_dup2(int old, int new)
{
    struct process *proc = sched_curr()->owner;
    if (proc->fds[new].vnode || proc->fds[new].pipe) return -1;

    memcpy(&proc->fds[new], &proc->fds[old], sizeof(proc->fds[new]));
    return new;
}
