#include <cpu/Gdt/Tss.h>

#include <mm/Pmm.h>

#include <stdint.h>
#include <string.h>

namespace gdt
{
    extern uint64_t descriptors[5];
}

extern "C" void install_tss();

namespace tss
{
    struct [[gnu::packed]] TSS
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
    TSS tss;

    struct [[gnu::packed]] GDTDescriptor
    {
        uint16_t limit;
        uint16_t baseLow;
        uint8_t  baseMid;
        uint8_t  access;
        uint8_t  flags;
        uint8_t  baseHigh;
    };

    GDTDescriptor EncodeGDTDescriptor(uint32_t base, uint16_t limit, uint8_t access, uint8_t flags)
    {
        GDTDescriptor ret;

        ret.baseLow  = base & 0xFFFF;
        ret.baseMid  = (base >> 16) & 0xFF;
        ret.baseHigh = (base >> 24) & 0xFF;
        ret.limit    = limit;
        ret.access   = access;
        ret.flags    = flags;

        return ret;
    }

    void Init()
    {
        tss.ist1 = PMM::PhysToVirt(PMM::GetPage()) + PMM::PAGE_SIZE; // TODO: Replace with a proper per-thread kernel stack

        GDTDescriptor first = EncodeGDTDescriptor((uint64_t)&tss, sizeof(tss), 0x89, 0x40);

        memcpy(&gdt::descriptors[3], &first, sizeof(GDTDescriptor));
        gdt::descriptors[4] = ((uint64_t)&tss) >> 32;

        install_tss();
    }
}