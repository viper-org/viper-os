#include <cpu/interrupt/apic.h>
#include <cpu/asm.h>

#include <driver/hpet.h>

#include <atheris/mm/vm.h>

namespace atheris
{
    namespace amd64
    {
        namespace cpu
        {
            namespace apic
            {
                enum Registers
                {
                    APIC_ID         = 0x20,
                    APIC_EOI        = 0xB0,
                    APIC_ICR0       = 0x300,
                    APIC_ICR1       = 0x310,
                    APIC_LVT        = 0x320,
                    APIC_INIT_CNT   = 0x380,
                    APIC_CURR_CNT   = 0x390,
                    APIC_DIV_CONFIG = 0x3E0,
                };

                volatile uint8_t* lapic;

                void WriteRegister(int reg, std::uint32_t value)
                {
                    *((volatile std::uint32_t*)(lapic + reg)) = value;
                }

                std::uint32_t ReadRegister(int reg)
                {
                    return *((volatile std::uint32_t*)(lapic + reg));
                }

                void Init()
                {
                    lapic = reinterpret_cast<std::uint8_t*>(vm::GetVirtualAddress(GetPhysicalAddress()));
                    vm::MapPage(nullptr,
                        GetPhysicalAddress(),
                        reinterpret_cast<std::uint64_t>(lapic),
                        vm::flags::write | vm::flags::present);

                    WriteRegister(0xF0, 0x1FF);
                }

                void InitTimer(int tickSizeNs)
                {
                    WriteRegister(APIC_DIV_CONFIG, 0x3);

                    WriteRegister(APIC_INIT_CNT, 0xFFFFFFFF); // -1
                    hpet::Sleep(tickSizeNs * (int)1e6);

                    WriteRegister(APIC_LVT, 1 << 16); // Mask

                    std::uint32_t ticks = 0xFFFFFFFF - ReadRegister(APIC_CURR_CNT); // 10ms ticks

                    WriteRegister(APIC_LVT, 32 | 0x20000); // IRQ 0, periodic
                    WriteRegister(APIC_DIV_CONFIG, 0x3);
                    WriteRegister(APIC_INIT_CNT, ticks);
                }

                pm::physaddr GetPhysicalAddress()
                {
                    std::uint64_t msr = ReadMSR(MSR::IA32_APIC_BASE);

                    return msr & 0xffffff000;
                }

                int GetID()
                {
                    return (ReadRegister(APIC_ID) >> 24) & 0xF;
                }

                void SendIPI(int id, int vector, IPIMode::IPIMode mode)
                {
                    WriteRegister(APIC_ICR1, id << 24);
                    WriteRegister(APIC_ICR0, (mode << 18) | vector);
                }

                void SendEOI()
                {
                    WriteRegister(APIC_EOI, 0);
                }
            }
        }
    }
}