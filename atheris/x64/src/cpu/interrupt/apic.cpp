#include <cpu/interrupt/apic.h>
#include <cpu/asm.h>

#include <mm/util.h>

namespace x64
{
    namespace cpu
    {
        namespace apic
        {
            enum Registers
            {
                APIC_ICR0 = 0x300,
                APIC_ICR1 = 0x310,
            };

            volatile uint8_t* lapic;

            void WriteRegister(int reg, uint32_t value)
            {
                *((volatile uint32_t*)(lapic + reg)) = value;
            }

            uint32_t ReadRegister(int reg)
            {
                return *((volatile uint32_t*)(lapic + reg));
            }

            void Init()
            {
                lapic = reinterpret_cast<uint8_t*>(PhysToVirt(GetPhysicalAddress()));

                WriteRegister(0xF0, 0x1FF);
            }

            echis::pmm::physaddr GetPhysicalAddress()
            {
                uint64_t msr = ReadMSR(MSR::IA32_APIC_BASE);

                return msr & 0xffffff000;
            }

            void SendIPI(int id, int vector, int mode)
            {
                WriteRegister(APIC_ICR1, id << 24);
                WriteRegister(APIC_ICR0, mode | vector);
            }
        }
    }
}