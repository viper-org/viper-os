#include <atheris/private/cpu/init.h>

#include <cpu/interrupt/apic.h>
#include <cpu/syscall.h>
#include <cpu/gdt/tss.h>
#include <cpu/core.h>
#include <cpu/asm.h>

#include <mm/vm.h>
#include <mm/util.h>

#include <driver/acpi.h>
#include <driver/hpet.h>

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

            x64::cpu::apic::Init();
            core::CoreLocal* core = new core::CoreLocal(x64::cpu::apic::GetID());
            x64::cpu::WriteMSR(x64::cpu::MSR::GSBase, reinterpret_cast<uint64_t>(core));

            x64::acpi::Init();
            x64::hpet::Init();
            x64::cpu::tss::Install();
            x64::cpu::EnableSyscall();
        }
    }
}