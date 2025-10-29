#include "cpu/tss.h"
#include "cpu/gdt.h"

#include <string.h>

struct __attribute__((packed)) gdt_descriptor
{
    uint16_t limit;
    uint16_t baseLow;
    uint8_t baseMid;
    uint8_t access;
    uint8_t flags;
    uint8_t baseHigh;
};

struct __attribute__((packed)) tss_descriptor
{
    uint32_t res0;

    uint64_t rsp0;
    uint64_t rsp1;
    uint64_t rsp2;

    uint32_t res1;
    uint32_t res2;

    uint64_t ist1;
    uint64_t ist2;
    uint64_t ist3;
    uint64_t ist4;
    uint64_t ist5;
    uint64_t ist6;
    uint64_t ist7;

    uint32_t res3;
    uint32_t res4;

    uint32_t iomap_base;
};

extern struct gdt_descriptor gdt[7];
extern struct gdt_descriptor init_descriptor(uint32_t base, uint16_t limit, uint8_t access, uint8_t flags);
extern void tss_install(void);

struct tss_descriptor tss;

void tss_init(void)
{
    memset(&tss, 0, sizeof tss);
    uint32_t tss_low = (uint64_t)&tss & 0xFFFFFFFF;
    uint32_t tss_high = ((uint64_t)&tss >> 32) & 0xFFFFFFFF;

    gdt[5] = init_descriptor(tss_low, sizeof(tss), 0x89, 0);
    memcpy(&gdt[6], &tss_high, sizeof tss_high);

    tss_install();
}

void tss_set_rsp0(uint64_t rsp0)
{
    tss.rsp0 = rsp0;
}
