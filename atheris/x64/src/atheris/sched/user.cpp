#include <atheris/private/sched/user.h>

#include <cpu/gdt/tss.h>
#include <cpu/core.h>

#include <echis/sched/process.h>

namespace atheris
{
    namespace sched
    {
        extern "C" void enter_usermode(uint64_t rip, uint64_t stack);

        void UserInit(echis::sched::Thread* thread)
        {
            thread->getParent()->getAddressSpace().switchTo();
            x64::cpu::tss::SetRSP0(thread->getKernelStack().top);
            cpu::core::CoreLocal::Get()->kernelStack = thread->getKernelStack().top;

            enter_usermode(thread->getStartingAddress(), thread->getUserStack().top);
        }
    }
}