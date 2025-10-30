#include "syscall/syscalls.h"

#include "sched/proc.h"
#include "sched/sched.h"

#include "driver/dbg.h"

#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2

long sys_lseek(int fd, long offset, int whence)
{
    struct process *proc = sched_curr()->owner;
    struct proc_fd *desc = &proc->fds[fd];
    
    switch (whence)
    {
        case SEEK_SET:
            desc->seek = offset;
            break;
        
        case SEEK_CUR:
            desc->seek += offset;
            break;
        
        // todo: SEEK_END
    }
    return desc->seek;
}
