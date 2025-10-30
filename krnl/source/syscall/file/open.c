#include "syscall/syscalls.h"

#include "sched/proc.h"
#include "sched/sched.h"

#include "mm/kheap.h"

#include "driver/dbg.h"

#include <string.h>

struct vnode *create_one(const char *pn)
{
    const char *component = strrchr(pn, '/')+1;
    char *parent = strdup(pn);
    parent[component - pn] = '\0';
    struct vnode *node = lookuppn(parent);
    if (!node)
    {
        node = create_one(parent);
    }
    struct vnode *out;
    node->fs->create(node, component, &out);
    kheap_free(parent);
    return out;
}

int sys_open(const char *path, uint16_t openmode)
{
    // todo: verify path
    struct process *proc = sched_curr()->owner;
    struct vnode *node = lookuppn(path);
    if (!node)
    {
        if (openmode & O_CREAT)
        {
            node = create_one(path); 
        }
    }
    int ret = proc_addfd(proc, node, openmode);
    dbg_printf("OPEN syscall executed with path=%s,mode=%d\nreturning fd=%d\n\n", path, openmode, ret);
    return ret;
}
