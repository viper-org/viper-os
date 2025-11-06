#include "acpi/madt.h"
#include "acpi/acpi.h"

struct madt *madt;
struct ioapic_header *ioapic = NULL;

void madt_init(void)
{
    madt = acpi_find_table("APIC");

    struct variable_record_header *hdr = (void *)((char *)madt + sizeof(struct madt));

    while (hdr->type != 1)
    {
        hdr = (void *)((char *)hdr + hdr->length);
    }
    ioapic = (void *)hdr;
}

uint32_t madt_ioapic_base(void)
{
    return ioapic->address;
}

uint32_t madt_gsi_base(void)
{
    return ioapic->gsi_base;
}

uint32_t madt_get_gsi(uint8_t source)
{
    struct variable_record_header *hdr = (void *)((char *)madt + sizeof(struct madt));

    while ((char *)hdr < (char *)madt + madt->header.length)
    {
        hdr = (void *)((char *)hdr + hdr->length);
        if (hdr->type == 2)
        {
            struct source_override_header *shdr = (void *)hdr;
            if (shdr->irq_soruce == source) return shdr->gsi;
        }
    }
    return source;
}
