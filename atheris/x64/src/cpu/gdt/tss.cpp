#include <cpu/gdt/tss.h>
#include <cpu/gdt/gdt.h>

#include <atheris/cpu/core.h>

#include <string.h>

namespace atheris
{
    namespace x64
    {
        namespace cpu
        {
            namespace gdt
            {
                extern gdt::Descriptor gdt[];
            }

            namespace tss
            {
                extern "C" void install_tss();
                
                void Install()
                {
                    TSSDescriptor& tss = atheris::cpu::core::CoreLocal::Get()->tssDesc;
                    uint32_t tssLow  = reinterpret_cast<uint64_t>(&tss) & 0xFFFFFFFF;
                    uint32_t tssHigh = (reinterpret_cast<uint64_t>(&tss) >> 32) & 0xFFFFFFFF;

                    gdt::gdt[5] = gdt::Descriptor(tssLow, sizeof(tss), 0x89, 0);
                    memcpy(&gdt::gdt[6], &tssHigh, sizeof(tssHigh));

                    install_tss();
                }

                void SetRSP0(uint64_t newStack)
                {
                    TSSDescriptor& tss = atheris::cpu::core::CoreLocal::Get()->tssDesc;
                    tss.rsp0 = newStack;
                }
            }
        }
    }
}