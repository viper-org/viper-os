#include <cpu/interrupt/apic.h>
#include <cpu/context.h>
#include <cpu/core.h>
#include <cpu/smp.h>
#include <cpu/halt.h>

#include <driver/timer.h>

#include <sched/signal.h>

#include <echis/signal/signal.h>

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

            if (auto signal = echis::signal::CheckIncoming())
            {
                sched::SavedUserContext userContext = {
                    context->BaseFrame.rip, context->BaseFrame.rsp, context->BaseFrame.rbp, context->BaseFrame.rflags,
                    context->GeneralRegisters.rax, context->GeneralRegisters.rbx, context->GeneralRegisters.rcx,
                    context->GeneralRegisters.rdx, context->GeneralRegisters.rsi, context->GeneralRegisters.rdi,
                    context->GeneralRegisters.r8, context->GeneralRegisters.r9, context->GeneralRegisters.r10,
                    context->GeneralRegisters.r11, context->GeneralRegisters.r12, context->GeneralRegisters.r13,
                    context->GeneralRegisters.r14, context->GeneralRegisters.r15
                };
                void(*handler)(int) = echis::signal::GetHandler(signal);
                signal->signum = echis::signal::NONE;
                sched::SignalHandler(userContext, handler);
            }
        }
    }
}