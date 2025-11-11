#include "driver/dbg.h"
#include "sched/procfd.h"
#include "syscall/syscalls.h"

#include "fs/vfs.h"

#include "sched/proc.h"
#include "sched/sched.h"

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
        else return -1;
    }
    if (node->type == VNODE_DIR)
    {
        if (openmode & O_DIRECTORY)
        {
            dbg_printf("hello: %s, %d\n", path, node->type);
            int ret = proc_addfd(proc, node, openmode);
            return ret;
        }
        return -1;
    }
    int ret = proc_addfd(proc, node, openmode);
    return ret;
}
