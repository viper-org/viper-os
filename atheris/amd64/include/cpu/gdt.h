#ifndef VIPEROS_ATHERIS_AMD64_CPU_GDT_H
#define VIPEROS_ATHERIS_AMD64_CPU_GDT_H 1

#include <cstdint>

namespace atheris
{
    namespace amd64
    {
        namespace cpu
        {
            namespace gdt
            {
                struct [[gnu::packed]] Descriptor
                {
                    std::uint16_t limit;
                    std::uint16_t baseLow;
                    std::uint8_t  baseMid;
                    std::uint8_t  access;
                    std::uint8_t  flags;
                    std::uint8_t  baseHigh;

                    Descriptor(std::uint32_t base, std::uint16_t limit, std::uint8_t access, std::uint8_t flags);
                    Descriptor() = default;
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

#endif // VIPEROS_ATHERIS_AMD64_CPU_GDT_H