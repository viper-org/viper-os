#include "cpu/int/lapic.h"

#include "cpu/asm.h"

#include "acpi/hpet.h"

#include "mm/vm.h"

enum
{
    APIC_ID         = 0x20,
    APIC_EOI        = 0xB0,
    APIC_ICR0       = 0x300,
    APIC_ICR1       = 0x310,
    APIC_LVT        = 0x320,
    APIC_INIT_CNT   = 0x380,
    APIC_CURR_CNT   = 0x390,
    APIC_DIV_CONFIG = 0x3E0,
};

static volatile uint8_t *lapic;

void write_lapic(int reg, uint32_t val)
{
    *((volatile uint32_t *)(lapic + reg)) = val;
}

uint32_t read_lapic(int reg)
{
    return *((volatile uint32_t *)(lapic + reg));
}

void lapic_init(void)
{
    lapic = vm_phystovirt(lapic_phys_addr());
    vm_map_page(NULL, lapic_phys_addr(), (uint64_t)lapic, PT_PRESENT | PT_WRITE);

    write_lapic(0xF0, 0x1FF);
}

void lapic_init_timer(void)
{
    write_lapic(APIC_DIV_CONFIG, 0x3);

    write_lapic(APIC_INIT_CNT, 0xFFFFFFFF);
    hpet_spin(10e12); // 10ms

    write_lapic(APIC_LVT, 1 << 16);

    uint32_t ticks = 0xFFFFFFFF - read_lapic(APIC_CURR_CNT);
    write_lapic(APIC_LVT, 32 | 0x20000);
    write_lapic(APIC_DIV_CONFIG, 3);
    write_lapic(APIC_INIT_CNT, ticks);
}

uint64_t lapic_phys_addr(void)
{
    uint64_t msr = rdmsr(IA32_APIC_BASE);
    return msr & 0xFFFFFF000;
}

void lapic_eoi(void)
{
    write_lapic(APIC_EOI, 0);
}
