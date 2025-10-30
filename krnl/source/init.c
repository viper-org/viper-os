#include "driver/ldr/loader.h"
#include "driver/dbg.h"

#include "cpu/cpu.h"

#include "event/bus.h"
#include "event/object.h"
#include "ldr/elf.h"
#include "mm/pm.h"
#include "mm/vm.h"
#include "mm/kheap.h"

#include "fs/testfs.h"
#include "fs/devfs.h"
#include "fs/initrd.h"
#include "fs/vfs.h"

#include "sched/proc.h"
#include "sched/sched.h"

#include <string.h>

extern char userproc[];
extern char userproc_end[];

void _start(void)
{
    cpu_earlyinit();

    pm_init();
    vm_init();
    kheap_init();
    vm_allocator_init();

    cpu_init();

    testfs_init();
    devfs_init();
    
    struct vnode *root = lookuppn("/");
    struct vnode *tmp;
    root->fs->mkdir(root, "dev", &tmp);
    devfs_get()->mount("/dev");

    dbg_writechar('\n');
    
    ldr_init();
    initrd_init();

    struct process *proc = alloc_proc((uint64_t)userproc);

    void *mem = vm_getpages(NULL, 16);
    size_t count = 0x10000;
    tmp = lookuppn("/tmp/usertest");
    tmp->fs->read(tmp, mem, &count, 0);

    struct elf_exec e = load_elf(mem, &proc->addr_space);
    proc->main_thread.entry = e.entry;
    sched_addproc(proc);
    
    sched_start();
    
    __asm__("cli; hlt");
}
