#include "syscall/syscalls.h"

#include "sched/proc.h"
#include "sched/sched.h"

int sys_munmap(void *addr, size_t len)
{
    struct process *proc = sched_curr()->owner;

    // todo: check if addr was actually allocated with mmap()
    vm_freepages(&proc->addr_space, addr, NPAGES(len));

    return 0;
}
