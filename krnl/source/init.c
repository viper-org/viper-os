#include "driver/ldr/loader.h"

#include "cpu/gdt.h"
#include "cpu/idt.h"

#include "fs/vfs.h"
#include "mm/pm.h"
#include "mm/vm.h"
#include "mm/kheap.h"

#include "fs/testfs.h"
#include "fs/devfs.h"

#include "limine.h"
#include <string.h>

static volatile struct limine_module_request modreq = {
    .id = LIMINE_MODULE_REQUEST,
    .revision = 1,
    .response = NULL
};

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
    
    ldr_init();
    for (uint64_t i = 0; i < modreq.response->module_count; ++i)
    {
        if (!strcmp("/sample.vdrv", modreq.response->modules[i]->path))
        {
            struct driver hdr = ldr_load(modreq.response->modules[i]->address);
            hdr.init();
            devfs_add_drv(hdr);
        }
    }

    tmp = lookuppn("/dev/sample");
    tmp->fs->read(tmp, "", 0);
    
    __asm__("cli; hlt");
}
