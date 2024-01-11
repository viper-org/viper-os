#include <atheris/private/cpu/init.h>

#include <cpu/interrupt/apic.h>

#include <mm/vm.h>
#include <mm/util.h>

#include <driver/acpi.h>

namespace atheris
{
    namespace cpu
    {
        void Init()
        {
            vm::MapPage(nullptr,
                            x64::cpu::apic::GetPhysicalAddress(),
                            x64::PhysToVirt(x64::cpu::apic::GetPhysicalAddress()),
                            vm::flags::present | vm::flags::write);
            x64::acpi::Init();
        }
    }
}