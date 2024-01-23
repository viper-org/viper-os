#include <cpu/context.h>

#include <mm/vm.h>

#include <sched/signal.h>

#include <echis/signal/signal.h>

#include <echis/core/exception/exception.h>

namespace atheris
{
    namespace x64
    {
        extern "C" void CommonExceptionHandler(cpu::Context* context)
        {
            echis::exception::Exception exceptionType;
            switch(context->BaseFrame.vector)
            {
                case 0:
                    exceptionType = echis::exception::DIV;
                    break;
                case 14:
                    if (!atheris::vm::HandlePageFault(context))
                    {
                        return;
                    }
                    exceptionType = echis::exception::SEG;
                    break;
                case 6:
                    exceptionType = echis::exception::ILL;
                    break;
                case 1:
                case 3:
                    exceptionType = echis::exception::BRK;
                    break;
                case 2:
                case 4:
                case 5:
                case 7:
                case 8:
                case 9:
                case 10:
                case 11:
                case 12:
                case 13:
                case 16:
                case 17:
                case 18:
                case 19:
                case 20:
                case 21:
                case 28:
                case 29:
                case 30:
                    exceptionType = echis::exception::ARC;
            }
            if ((context->BaseFrame.cs & 0x3) == 0x3) // We came from user-space
            {
                if (exceptionType == echis::exception::SEG)
                {
                    if (context->ControlRegisters.cr2 == 0x516) // SIG - return from signal handler
                    {
                        sched::ReturnFromSignalHandler(context->BaseFrame.rsp);
                    }

                    echis::signal::DeliverToCurrent(echis::signal::SIGSEGV);
                }
                else
                {
                    echis::exception::raise(exceptionType, context->BaseFrame.error_code);
                }
            }
            else
            {
                echis::exception::raise(exceptionType, context->BaseFrame.error_code);
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