#include "driver/ldr/loader.h"
#include "driver/dbg.h"

#include "cpu/gdt.h"
#include "cpu/idt.h"

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

void threadmain(void)
{
    while (1) {
        dbg_writechar('A');
        sched_yield();
    }
}

void threadmain2(void)
{
    while (1) {
        dbg_writechar('B');
        sched_yield();
    }
}

void _start(void)
{
    gdt_init();
    idt_init();

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

    struct process *proc = alloc_proc((uint64_t)threadmain);
    struct process *proc2 = alloc_proc((uint64_t)threadmain2);
    sched_addproc(proc);
    sched_addproc(proc2);
    sched_start();
    
    __asm__("cli; hlt");
}
