#include "sched/procfd.h"
#include "syscall/syscalls.h"

#include "fs/vfs.h"

#include "sched/proc.h"
#include "sched/sched.h"

#include "mm/kheap.h"

int sys_open(const char *path, uint16_t openmode)
{
    // todo: verify path
    struct process *proc = sched_curr()->owner;
    char *fullpath = get_relpath(path, proc->cwd);
    struct vnode *node = lookuppn(fullpath);
    kheap_free(fullpath);

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
            int ret = proc_addfd(proc, node, openmode);
            return ret;
        }
        return -1;
    }
    int ret = proc_addfd(proc, node, openmode);
    return ret;
}
