#include "cpu/int/ioapic.h"

#include "acpi/madt.h"

#include "mm/vm.h"

static uint32_t ioapic_phys;
static volatile void *ioapic;
static uint32_t gsi_base = 0;

static inline void write_ioapic(uint8_t offset, uint32_t val)
{
    *(volatile uint32_t *)(ioapic) = offset;
    *(volatile uint32_t *)((char *)ioapic + 0x10) = val;
}

static inline uint32_t read_ioapic(uint8_t offset)
{
    *(volatile uint32_t *)(ioapic) = offset;
    return *(volatile uint32_t *)((char *)ioapic + 0x10);
}

void ioapic_init(void)
{
    ioapic_phys = madt_ioapic_base();
    ioapic = vm_phystovirt(ioapic_phys);
    vm_map_page(NULL, ioapic_phys, (uint64_t)ioapic, PT_PRESENT | PT_WRITE);
    gsi_base = madt_gsi_base();

    union redir_entry kb_ent;
    kb_ent.vector = 0x21;
    kb_ent.delv_mode = 0;
    kb_ent.dest_mode = 0;
    kb_ent.delv_status = 0;
    kb_ent.pin_polarity = 0;
    kb_ent.remote_irr = 0;
    kb_ent.trigger_mode = 0;
    kb_ent.mask = 0;
    kb_ent.reserved = 0;
    kb_ent.dest_mode = 0;
    ioapic_add_redir(madt_get_gsi(1), kb_ent);
}

void ioapic_add_redir(uint8_t no, union redir_entry ent)
{
    write_ioapic(0x10 + 2 * no, ent.low);
    write_ioapic(0x10 + 2 * no + 1, ent.high);
}
