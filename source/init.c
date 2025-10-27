#include "driver/dbg.h"

#include "mm/pm.h"
#include "mm/vm.h"

void _start(void)
{
    pm_init();
    vm_init();
    physaddr_t page = pm_getpage();
    int* mem = vm_phystovirt(page);
    *mem = 12;
    dbg_writechar('\n');
    dbg_printf("%p - %d", mem, *mem);
    pm_freepage(page);
    __asm__("cli; hlt");
}
