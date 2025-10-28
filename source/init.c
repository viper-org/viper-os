#include "driver/dbg.h"

#include "cpu/gdt.h"
#include "cpu/idt.h"

#include "mm/pm.h"
#include "mm/vm.h"
#include "mm/kheap.h"

#include "fs/testfs.h"

void _start(void)
{
    gdt_init();
    idt_init();

    pm_init();
    vm_init();
    kheap_init();
    vm_allocator_init();

    testfs_init();
    dbg_writechar('\n');
    struct vnode *rootnode = lookuppn("/");
    struct vnode *file = NULL;
    struct vnode *dir = NULL;
    rootnode->fs->mkdir(rootnode, "test", &dir);
    rootnode->fs->create(dir, "file", &file);
    rootnode->fs->write(file, "hello", 6);

    char buf[6];
    file = lookuppn("/test/file");
    rootnode->fs->read(file, buf, 6);
    dbg_print(buf);
    
    __asm__("cli; hlt");
}
