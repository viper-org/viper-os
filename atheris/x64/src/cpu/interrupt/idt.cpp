#include <cpu/interrupt/idt.h>

#include <echis/driver/console.h>

namespace x64
{
    namespace cpu
    {
        namespace interrupt
        {
            extern "C" uint64_t exception_stub_table[];

            Descriptor::Descriptor(uint64_t isr, uint8_t flags, uint8_t ist)
                : offsetLow(isr & 0xffff)
                , kernelCS(0x08)
                , ist(ist)
                , attributes(flags)
                , offsetMid((isr >> 16)  & 0xffff)
                , offsetHigh((isr >> 32) & 0xffffffff)
                , reserved(0)
            {
            }

            Descriptor::Descriptor() {}

            constexpr uint8_t defaultFlags = 0x8e;
            constexpr int exceptionCount = 32;

            static Descriptor idt[256];
            static Pointer idtr;

            void Install()
            {
                idtr.limit = sizeof(idt) - 1;
                idtr.base = idt;

                for (int vector = 0; vector < exceptionCount; ++vector)
                {
                    idt[vector] = Descriptor(exception_stub_table[vector], defaultFlags, 0);
                }

                asm volatile("lidt %0; sti" :: "m"(idtr));
            }
        }
    }
}