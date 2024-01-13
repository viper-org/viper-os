#include <atheris/private/cpu/smp.h>
#include <atheris/private/cpu/core.h>
#include <atheris/private/cpu/halt.h>

#include <cpu/gdt/gdt.h>
#include <cpu/interrupt/idt.h>
#include <cpu/interrupt/apic.h>
#include <cpu/asm.h>
#include <cpu/syscall.h>

#include <driver/timer.h>

#include <mm/vm.h>
#include <mm/util.h>

#include <echis/std/container/atomic.h>

#include <limine.h>

#include <stdio.h>

namespace atheris
{
    namespace cpu
    {
        namespace smp
        {
            static volatile limine_smp_request smpRequest = {
                .id = LIMINE_SMP_REQUEST,
                .revision = 1,
                .response = nullptr
            };
            volatile vpr::atomic_bool nextAPDone;
            
            void APInit(limine_smp_info*);

            void Init()
            {
                nextAPDone = true;
                for (uint64_t i = 0; i < smpRequest.response->cpu_count; ++i)
                {
                    if (smpRequest.response->cpus[i]->lapic_id != smpRequest.response->bsp_lapic_id)
                    {
                        while(!nextAPDone);
                        nextAPDone = false;
                        smpRequest.response->cpus[i]->goto_address = &APInit;
                    }
                }
            }

            void APInit(limine_smp_info* coreInfo)
            {
                printf("CPU#%i started\n", coreInfo->lapic_id);
                nextAPDone = true;

                vm::APInstallKernelPageTables();
                x64::cpu::gdt::APInstall();
                x64::cpu::interrupt::APInstall();

                core::CoreLocal* core = new core::CoreLocal(coreInfo->lapic_id);
                x64::cpu::WriteMSR(x64::cpu::MSR::GSBase, reinterpret_cast<uint64_t>(core));
                //x64::cpu::tss::Install(); TODO: Make gdt core local
                x64::cpu::apic::Init();
                timer::Init();
                x64::cpu::EnableSyscall();

                Halt();
            }


            void SendIPI(int core, int vector, IPIDestination destination)
            {
                using namespace x64::cpu;
                apic::IPIMode::IPIMode mode = apic::IPIMode::Single;
                switch(destination)
                {
                    case IPIDestination::Self:
                        mode = apic::IPIMode::Self;
                        break;
                    case IPIDestination::BroadcastAll:
                        mode = apic::IPIMode::BroadcastAll;
                        break;
                    case IPIDestination::BroadcastOthers:
                        mode = apic::IPIMode::BroadcastOthers;
                        break;
                    default:
                        break;
                }
                apic::SendIPI(core, vector, mode);
            }
        }
    }
}