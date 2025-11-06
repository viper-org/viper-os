#include "driver/keyboard.h"
#include "driver/ldr/loader.h"
#include "driver/dbg.h"

#include "cpu/cpu.h"
#include "cpu/int/lapic.h"
#include "cpu/int/ioapic.h"

#include "acpi/acpi.h"
#include "acpi/hpet.h"
#include "acpi/madt.h"

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
#include "syscall/stat.h"

#include <string.h>

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

    acpi_init();
    hpet_init();
    madt_init();
    lapic_init();
    lapic_init_timer();
    ioapic_init();
    kb_init();

    struct process *proc = alloc_proc(0);
    struct addrspace *prev = vm_get_addrspace();
    vm_switch_to(&proc->addr_space);

    void *mem = vm_getpages(NULL, 16);
    tmp = lookuppn("/bin/tty");
    struct stat b;
    tmp->fs->stat(tmp, &b);
    size_t sz = b.st_size;
    tmp->fs->read(tmp, mem, &sz, 0);

    struct elf_exec e = load_elf(mem, &proc->addr_space);
    push_elf_auxvals(&e, proc->main_thread.usr_stack.top);
    proc->main_thread.entry = e.entry;
    sched_addproc(proc);

    vm_switch_to(prev);
    
    sched_start();
    
    __asm__("cli; hlt");
}
