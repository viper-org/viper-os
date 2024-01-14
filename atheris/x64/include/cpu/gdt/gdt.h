#ifndef VIPEROS_ATHERIS_X64_CPU_GDT_GDT_H
#define VIPEROS_ATHERIS_X64_CPU_GDT_GDT_H 1

#include <stdint.h>

namespace atheris
{
    namespace x64
    {
        namespace cpu
        {
            namespace gdt
            {
                struct [[gnu::packed]] Descriptor
                {
                    uint16_t limit;
                    uint16_t baseLow;
                    uint8_t  baseMid;
                    uint8_t  access;
                    uint8_t  flags;
                    uint8_t  baseHigh;

                    Descriptor(uint32_t base, uint16_t limit, uint8_t access, uint8_t flags);
                    Descriptor();
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
}

#endif // VIPEROS_ATHERIS_X64_CPU_GDT_GDT_H