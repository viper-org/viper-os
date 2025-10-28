#include "driver/dbg.h"

#include "cpu/gdt.h"
#include "cpu/idt.h"

#include "mm/pm.h"
#include "mm/vm.h"
#include "mm/kheap.h"

void _start(void)
{
    gdt_init();
    idt_init();

    pm_init();
    vm_init();
    kheap_init();
    vm_allocator_init();

    void* pages = vm_getpage(NULL);
    *(int*)pages = 55;
    dbg_printf("\n%d", *(int*)pages);
    *(int*)8293789 = 1;
    
    __asm__("cli; hlt");
}
