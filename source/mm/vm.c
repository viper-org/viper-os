#include "mm/vm.h"

#include <limine.h>

struct limine_hhdm_request hhdm_request = {
    .id = LIMINE_HHDM_REQUEST,
    .revision = 1,
    .response = NULL
};

void *vm_phystovirt(physaddr_t addr)
{
    return (void *)(addr + hhdm_request.response->offset);
}
