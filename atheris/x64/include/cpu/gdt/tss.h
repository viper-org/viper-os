#ifndef VIPEROS_ATHERIS_X64_CPU_GDT_TSS_H
#define VIPEROS_ATHERIS_X64_CPU_GDT_TSS_H 1

#include <stdint.h>

namespace atheris
{
    namespace x64
    {
        namespace cpu
        {
            namespace tss
            {
                struct [[gnu::packed]] TSSDescriptor
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

                void Install();

                void SetRSP0(uint64_t newStack);
            }
        }
    }
}

#endif // VIPEROS_ATHERIS_X64_CPU_GDT_TSS_H