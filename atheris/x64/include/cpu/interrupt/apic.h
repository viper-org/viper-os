#ifndef VIPEROS_ATHERIS_X64_CPU_INTERRUPT_APIC_H
#define VIPEROS_ATHERIS_X64_CPU_INTERRUPT_APIC_H 1

#include <echis/mm/physaddr.h>

#include <stdint.h>

namespace x64
{
    namespace cpu
    {
        namespace apic
        {
            struct [[gnu::packed]] ICR
            {
                uint8_t vector;
                uint64_t flags : 12;
                uint64_t reserved : 36;
                uint8_t destination;
            };

            void Init();
            echis::pmm::physaddr GetPhysicalAddress();

            void SendIPI(int id, int vector, int mode);
        }
    }
}

#endif // VIPEROS_ATHERIS_X64_CPU_INTERRUPT_APIC_H