#ifndef VIPEROS_ATHERIS_AMD64_CPU_INTERRUPT_IDT_H
#define VIPEROS_ATHERIS_AMD64_CPU_INTERRUPT_IDT_H 1

#include <cstdint>

namespace atheris
{
    namespace amd64
    {
        namespace cpu
        {
            namespace idt
            {
                struct [[gnu::packed]] Descriptor
                {
                    Descriptor(std::uint64_t isr, std::uint8_t flags, std::uint8_t ist);
                    Descriptor();

                    std::uint16_t offsetLow;
                    std::uint16_t kernelCS;
                    std::uint8_t  ist;
                    std::uint8_t  attributes;
                    std::uint16_t offsetMid;
                    std::uint32_t offsetHigh;
                    std::uint32_t reserved;
                };

                struct [[gnu::packed]] Pointer
                {
                    std::uint16_t limit;
                    Descriptor* base;
                };

                void Install();
            }
        }
    }
}

#endif // VIPEROS_ATHERIS_AMD64_CPU_INTERRUPT_IDT_H