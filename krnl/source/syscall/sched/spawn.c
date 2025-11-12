#include "driver/dbg.h"
#include "syscall/syscalls.h"

#include "mm/vm.h"
#include "mm/kheap.h"

#include "sched/proc.h"
#include "sched/sched.h"

#include "fs/vfs.h"

#include "ldr/elf.h"

#include <string.h>

int sys_spawn(const char *path, int argc, char **argv, struct spawn *spawnbuf)
{
    struct process *parent = sched_curr()->owner;

    struct vnode *node = lookuppn(path);
    struct stat b;
    node->fs->stat(node, &b);

    char *buf = vm_getpages(NULL, NPAGES(b.st_size), 0, NULL);
    size_t _;
    node->fs->read(node, buf, &_, 0);

    size_t total_arg_len = 0;
    for (int i = 0; i < argc; ++i)
    {
        total_arg_len += strlen(argv[i]) + 1;
    }
    char *argv_copy = kheap_alloc(total_arg_len);
    char *p = argv_copy;
    for (int i = 0; i < argc; ++i)
    {
        size_t len = strlen(argv[i]);
        memcpy(p, argv[i], len+1);
        p += len + 1;
    }
    for (int i = 0; i < total_arg_len; ++i)
    {
        dbg_writechar(argv_copy[i]);
    }

    struct process *proc = alloc_proc(parent->pid, parent->cwd);
    struct addrspace *prev = vm_get_addrspace();
    vm_switch_to(&proc->addr_space);

    struct elf_exec e = load_elf(buf, &proc->addr_space);
    proc->main_thread.ustack_save = push_argvc(argc, argv_copy, proc->main_thread.usr_stack.top, total_arg_len);
    push_elf_auxvals(&e, proc->main_thread.ustack_save);
    proc->main_thread.entry = e.entry;
    sched_addproc(proc);

    vm_switch_to(prev);

    kheap_free(argv_copy);

    memcpy(proc->fds, parent->fds, sizeof proc->fds);

    if (spawnbuf->infd >= 0)
        proc->fds[0] = parent->fds[spawnbuf->infd];
    if (spawnbuf->outfd >= 0)
        proc->fds[1] = parent->fds[spawnbuf->outfd];
    if (spawnbuf->errfd >= 0)
        proc->fds[2] = parent->fds[spawnbuf->errfd];

    return proc->pid;
}
