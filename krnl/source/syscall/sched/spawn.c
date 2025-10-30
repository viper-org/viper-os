#include "syscall/syscalls.h"

#include "mm/kheap.h"

#include "sched/proc.h"
#include "sched/sched.h"

#include "fs/vfs.h"

#include "ldr/elf.h"
#include <string.h>

int sys_spawn(const char *path)
{
    struct process *parent = sched_curr()->owner;

    struct vnode *node = lookuppn(path);
    struct stat b;
    node->fs->stat(node, &b);

    char *buf = kheap_alloc(b.st_size);
    size_t _;
    node->fs->read(node, buf, &_, 0);

    struct process *proc = alloc_proc(0);
    struct elf_exec e = load_elf(buf, &proc->addr_space);
    proc->main_thread.entry = e.entry;
    sched_addproc(proc);

    memcpy(proc->fds, parent->fds, sizeof proc->fds);

    return proc->pid;
}
