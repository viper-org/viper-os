#include "acpi/acpi.h"
#include "mm/vm.h"

#include <string.h>

#include <limine.h>

static volatile struct limine_rsdp_request rsdpRequest = {
    .id = LIMINE_RSDP_REQUEST_ID,
    .revision = 1,
    .response = NULL,
};

struct xsdt* xsdt = NULL;
struct rsdt* rsdt = NULL;

void acpi_init(void)
{
    struct rsdp *rsdp = (struct rsdp *)rsdpRequest.response->address;
    if(rsdp->revision == 2)
    {
        struct rsdp2 *rsdp2 = (struct rsdp2 *)rsdp;
        xsdt = vm_phystovirt(rsdp2->xsdt);
    }
    else
        rsdt = vm_phystovirt(rsdp->rsdt);
}

void* acpi_find_table(const char* signature)
{
    if(xsdt)
    {
        int entryCount = (xsdt->header.length - sizeof(xsdt->header)) / 8;

        for(int i = 0; i < entryCount; i++)
        {
            uint64_t *other = (uint64_t *)((uint8_t *)xsdt + sizeof(struct xsdt));
            struct sdt_header *hdr = vm_phystovirt(other[i]);

            if(!memcmp(hdr->signature, signature, 4))
            {
                return (void*)hdr;
            }
        }
    }
    else if(rsdt)
    {
        int entryCount = (rsdt->header.length - sizeof(rsdt->header)) / 4;

        for(int i = 0; i < entryCount; i++)
        {
            uint32_t *other = (uint32_t *)((uint8_t *)rsdt + sizeof(struct rsdt));
            struct sdt_header *hdr = vm_phystovirt(other[i]);
            if(!memcmp(hdr->signature, signature, 4))
                return (void*)hdr;
        }
    }
    return NULL;
}
