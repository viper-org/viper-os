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
            void InitTimer();
            echis::pmm::physaddr GetPhysicalAddress();

            namespace IPIMode
            {
                enum IPIMode
                {
                    Single          = 0b00,
                    Self            = 0b01,
                    BroadcastAll    = 0b10,
                    BroadcastOthers = 0b11
                };
            }

            void SendIPI(int id, int vector, IPIMode::IPIMode mode);

            void SendEOI();
        }
    }
}

#endif // VIPEROS_ATHERIS_X64_CPU_INTERRUPT_APIC_H