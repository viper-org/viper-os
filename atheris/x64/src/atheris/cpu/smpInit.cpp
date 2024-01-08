#include <cpu/smp.h>
#include <cpu/gdt/gdt.h>
#include <cpu/interrupt/idt.h>

#include <common/halt.h>

#include <mm/vm.h>

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
                printf("CPU#%i started\n", coreInfo->processor_id);
                nextAPDone = true;
                vm::APInstallKernelPageTables();
                x64::cpu::gdt::APInstall();
                x64::cpu::interrupt::APInstall();
                atheris::Halt();
            }
        }
    }
}