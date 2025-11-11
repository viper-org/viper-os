#include "syscall/syscalls.h"

#include "mm/vm.h"

#include "sched/sched.h"

enum flags
{
    MAP_ANONYMOUS = 1
};

void *sys_mmap(void *addr, size_t len, int prot, int flags, int fd, long offset)
{
    // todo: proper mmap implementation
    (void)addr;
    (void)prot;
    (void)flags;
    (void)offset;

    size_t pages = NPAGES(len);

    if (flags & MAP_ANONYMOUS)
    {
        void *r = vm_getpages(&sched_curr()->owner->addr_space, pages, 0, NULL);
        return r;
    }

    struct process *proc = sched_curr()->owner;
    struct vnode *node = proc->fds[fd].vnode;
    if (!node) return NULL;
    void *m = vm_getpages(&proc->addr_space, pages, 0, node);
    node->fs->mmap(node, &sched_curr()->owner->addr_space, m, len);
    return m;
}
