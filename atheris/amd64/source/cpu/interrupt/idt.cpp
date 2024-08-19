#include <cpu/interrupt/idt.h>

#include <echis/driver/debugcon.h>

namespace atheris
{
    namespace amd64
    {
        namespace cpu
        {
            namespace idt
            {
                extern "C" std::uint64_t exception_stub_table[];
                extern "C" uint64_t irq_stub_table[];

                Descriptor::Descriptor(std::uint64_t isr, std::uint8_t flags, std::uint8_t ist)
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

                constexpr std::uint8_t defaultFlags = 0x8E;
                constexpr int MaxException = 32;
                constexpr int MaxIRQ       = 256;

                static Descriptor idt[256];
                static Pointer idtr;

                void Install()
                {
                    idtr.limit = sizeof(idt) - 1;
                    idtr.base = idt;

                    for (int vector = 0; vector < MaxException; ++vector)
                    {
                        idt[vector] = Descriptor(exception_stub_table[vector], defaultFlags, 0);
                    }

                    for (int vector = MaxException; vector < MaxIRQ; ++vector)
                    {
                        idt[vector] = Descriptor(irq_stub_table[vector - MaxException], defaultFlags, 0);
                    }

                    asm volatile("lidt %0; sti" :: "m"(idtr));
                }
            }
        }
    }
}