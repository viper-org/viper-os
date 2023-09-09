#include <cpu/interrupt/Idt.h>
#include <cpu/interrupt/Exception.h>

#include <stdint.h>

namespace idt
{
    static IDTEntry idt[256];
    static IDTPointer idtr;

    static bool vectors[256];

    extern "C" uint64_t ExceptionStubTable[];
    extern "C" uint64_t IRQStubTable[];

    IDTEntry::IDTEntry(uint64_t isr, uint8_t flags, uint8_t ist)
    {
        OffsetLow = isr & 0xFFFF;
        KernelCS = 0x8;
        IST = ist;
        Attributes = flags;
        OffsetMid = (isr >> 16) & 0xFFFF;
        OffsetHigh = (isr >> 32) & 0xFFFFFFFF;
        Reserved = 0;
    }
    IDTEntry::IDTEntry()
        :OffsetLow(0), KernelCS(0), IST(0), Attributes(0), OffsetMid(0), OffsetHigh(0), Reserved(0)
    {}

    constexpr uint8_t DEFAULT_FLAGS = 0x8E;

    void Init()
    {
        idtr.Limit = sizeof(idt) - 1;
        idtr.Base = &idt[0];

        for(uint8_t vector = 0; vector < 32; vector++)
        {
            idt[vector] = IDTEntry(ExceptionStubTable[vector], DEFAULT_FLAGS, 0);
            vectors[vector] = true;
        }
        for(uint8_t vector = 0; vector < 16; vector++)
        {
            idt[vector + 32] = IDTEntry(IRQStubTable[vector], DEFAULT_FLAGS, 0);
            vectors[vector + 32] = true;
        }

        exception::Init();

        asm volatile("lidt %0" : : "m"(idtr));
        asm volatile("sti");
    }
}