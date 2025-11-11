#include "syscall/syscalls.h"

#include "fs/vfs.h"

#include "mm/kheap.h"

#include "sched/sched.h"

int sys_mkdir(const char *path)
{
    struct process *proc = sched_curr()->owner;
    char *fullpath = get_relpath(path, proc->cwd);

    int ret = recursive_create(fullpath, 1) ? 0 : -1;

    kheap_free(fullpath);

    return ret;
}
