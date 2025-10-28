#include <cpu/gdt.h>

#include <stdint.h>

struct __attribute__((packed)) gdt_descriptor
{
    uint16_t limit;
    uint16_t baseLow;
    uint8_t baseMid;
    uint8_t access;
    uint8_t flags;
    uint8_t baseHigh;
};

struct __attribute__((packed)) gdt_pointer
{
    uint16_t limit;
    struct gdt_descriptor *base;
};

void gdt_install(struct gdt_pointer *gdtr);

struct gdt_descriptor gdt[3];
struct gdt_pointer gdtr;

struct gdt_descriptor init_descriptor(uint32_t base, uint16_t limit, uint8_t access, uint8_t flags)
{
    struct gdt_descriptor ret;

    ret.limit = limit;
    ret.baseLow = base & 0xFFFF;
    ret.baseMid = (base >> 16) & 0xFF;
    ret.access = access;
    ret.flags = flags;
    ret.baseHigh = (base >> 24) & 0xFF;

    return ret;
}

void gdt_init(void)
{
    gdt[0] = init_descriptor(0, 0, 0, 0);

    gdt[1] = init_descriptor(0, 0, 0x9A, 0xA0);
    gdt[2] = init_descriptor(0, 0, 0x92, 0xC0);

    gdtr.limit = sizeof(gdt) - 1;
    gdtr.base = gdt;

    gdt_install(&gdtr);
}
