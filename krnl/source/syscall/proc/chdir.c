#include "syscall/syscalls.h"

#include "sched/proc.h"
#include "sched/sched.h"

#include "mm/kheap.h"

#include "string.h"

int sys_chdir(char *path)
{
    struct process *curr = sched_curr()->owner;

    char *new = get_relpath(path, curr->cwd);
    struct vnode *node = lookuppn(new);
    if (!node || node->type != VNODE_DIR)
    {
        kheap_free(new);
        return -1;
    }
    kheap_free(curr->cwd);
    curr->cwd = new;
    return 0;
}
