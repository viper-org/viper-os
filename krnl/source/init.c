#include "driver/ldr/loader.h"

#include "cpu/gdt.h"
#include "cpu/idt.h"

#include "mm/pm.h"
#include "mm/vm.h"
#include "mm/kheap.h"

#include "fs/testfs.h"

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
    
    ldr_init();
    for (uint64_t i = 0; i < modreq.response->module_count; ++i)
    {
        if (!strcmp("/sample.vdrv", modreq.response->modules[i]->path))
        {
            struct driver hdr = ldr_load(modreq.response->modules[i]->address);
            hdr.init();
        }
    }
    
    __asm__("cli; hlt");
}
