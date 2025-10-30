#include "syscall/syscalls.h"

#include "sched/proc.h"
#include "sched/sched.h"

#include "mm/kheap.h"

#include "driver/dbg.h"

#include <string.h>

int sys_open(const char *path, uint16_t openmode)
{
    // todo: verify path
    struct process *proc = sched_curr()->owner;
    struct vnode *node = lookuppn(path);
    if (!node)
    {
        if (openmode & O_CREAT)
        {
            node = recursive_create(path, 0); 
        }
    }
    int ret = proc_addfd(proc, node, openmode);
    dbg_printf("OPEN syscall executed with path=%s,mode=%d\nreturning fd=%d\n\n", path, openmode, ret);
    return ret;
}
