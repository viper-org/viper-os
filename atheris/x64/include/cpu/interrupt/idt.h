#ifndef VIPEROS_ATHERIS_X64_CPU_INTERRUPT_IDT_H
#define VIPEROS_ATHERIS_X64_CPU_INTERRUPT_IDT_H 1

#include <stdint.h>

namespace x64
{
    namespace cpu
    {
        namespace interrupt
        {
            struct [[gnu::packed]] Descriptor
            {
                Descriptor(uint64_t isr, uint8_t flags, uint8_t ist);
                Descriptor();

                uint16_t offsetLow;
                uint16_t kernelCS;
                uint8_t  ist;
                uint8_t  attributes;
                uint16_t offsetMid;
                uint32_t offsetHigh;
                uint32_t reserved;
            };

            struct [[gnu::packed]] Pointer
            {
                uint16_t limit;
                Descriptor* base;
            };

            void Install();
            void APInstall();
        }
    }
}

#endif // VIPEROS_ATHERIS_X64_CPU_INTERRUPT_IDT_H