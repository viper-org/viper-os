#include "syscall/syscalls.h"

#include "mm/vm.h"

#include "sched/sched.h"

void *sys_mmap(void *addr, size_t len, int prot, int flags, int fd, long offset)
{
    // todo: proper mmap implementation
    (void)addr;
    (void)prot;
    (void)flags;
    (void)fd;
    (void)offset;

    size_t pages = NPAGES(len);

    return vm_getpages(&sched_curr()->owner->addr_space, pages);
}
