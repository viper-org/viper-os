#include "driver/ldr/loader.h"
#include "driver/dbg.h"

#include "cpu/cpu.h"

#include "event/bus.h"
#include "event/object.h"
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

extern void userproc();
extern void userproc_end();

void _start(void)
{
    cpu_init();

    pm_init();
    vm_init();
    kheap_init();
    vm_allocator_init();

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
    void *page = vm_getpage(&proc->addr_space);
    proc->main_thread.entry = (uint64_t)page;
    vm_switch_to(&proc->addr_space);
    memcpy(page, userproc, userproc_end - userproc);
    sched_addproc(proc);
    sched_start();
    
    __asm__("cli; hlt");
}
