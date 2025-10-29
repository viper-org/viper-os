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

struct gdt_descriptor gdt[7];
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

    gdt[1] = init_descriptor(0, 0, 0x9A, 0xA0); // kernel code
    gdt[2] = init_descriptor(0, 0, 0x92, 0xC0); // kernel data

    gdt[3] = init_descriptor(0, 0, 0x92 | 0x60, 0xC0); // user data
    gdt[4] = init_descriptor(0, 0, 0x9A | 0x60, 0xA0); // user code

    gdt[5] = init_descriptor(0, 0, 0, 0);
    gdt[6] = init_descriptor(0, 0, 0, 0);

    gdtr.limit = sizeof(gdt) - 1;
    gdtr.base = gdt;

    gdt_install(&gdtr);
}
