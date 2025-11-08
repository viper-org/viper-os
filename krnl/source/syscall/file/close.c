#include "syscall/syscalls.h"

#include "sched/proc.h"
#include "sched/sched.h"

#include <string.h>

int sys_close(int fd)
{
    struct process *proc = sched_curr()->owner;
    struct proc_fd *desc = &proc->fds[fd];
    memset(desc, 0, sizeof *desc);
    // todo: free vnode if required
    return 0;
}
