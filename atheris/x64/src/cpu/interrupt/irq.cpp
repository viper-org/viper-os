#include <cpu/interrupt/apic.h>
#include <cpu/context.h>
#include <cpu/core.h>
#include <cpu/smp.h>
#include <cpu/halt.h>

#include <driver/timer.h>

#include <stdio.h>

namespace atheris
{
    namespace x64
    {
        extern "C" void CommonIRQHandler(cpu::Context* context)
        {
            x64::cpu::apic::SendEOI();
            if (context->BaseFrame.vector == ::atheris::cpu::smp::IPI::Panic)
            {
                int id = atheris::cpu::core::id;
                printf("%#Panic IPI received on CPU#%i\n", 0xff0000, id);
                atheris::cpu::Halt();
            }
            else if (context->BaseFrame.vector == atheris::timer::TimerInterruptVector)
            {
                atheris::timer::TimerInterruptHandler();
            }
        }
    }
}