#include "cpu/idt.h"

#include <stdint.h>

extern uint64_t exception_stub_table[];

struct __attribute__((packed)) idt_descriptor
{
    uint16_t offLow;
    uint16_t kCS;
    uint8_t ist;
    uint8_t attrs;
    uint16_t offMid;
    uint32_t offHigh;
    uint32_t reserved;
};

struct __attribute__((packed)) idt_pointer
{
    uint16_t limit;
    struct idt_descriptor *base;
};

struct idt_descriptor init_idt_desc(uint64_t isr, uint8_t flags, uint8_t ist)
{
    return (struct idt_descriptor) {
        .offLow = isr & 0xFFFF,
        .kCS = 8,
        .ist = ist,
        .attrs = flags,
        .offMid = (isr >> 16) & 0xFFFF,
        .offHigh = (isr >> 32) & 0xFFFFFFFF,
        .reserved = 0
    };
}

static struct idt_descriptor idt[256];
static struct idt_pointer idtr;

void idt_init(void)
{
    idtr.limit = sizeof idt - 1;
    idtr.base = idt;

    for (int vec = 0; vec < 32; ++vec)
    {
        idt[vec] = init_idt_desc(exception_stub_table[vec], 0x8E, 0);
    }

    // todo: sti
    __asm__ volatile("lidt %0" :: "m"(idtr));
}
