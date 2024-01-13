#include <atheris/private/sched/user.h>

#include <cpu/gdt/tss.h>
#include <cpu/core.h>

#include <echis/sched/sched.h>
#include <echis/sched/process.h>

namespace atheris
{
    namespace sched
    {
        extern "C" void enter_usermode(uint64_t rip, uint64_t stack);
        extern "C" void prepare_thread(uint64_t func, ThreadContext** context, uint64_t kernelStack, echis::sched::Thread* thread);
        extern "C" void switch_context(ThreadContext** oldContext, ThreadContext* newContext);

        void UserInit(echis::sched::Thread* thread)
        {
            thread->getParent()->getAddressSpace().switchTo();
            x64::cpu::tss::SetRSP0(thread->getKernelStack().top);
            cpu::core::CoreLocal::Get()->kernelStack = thread->getKernelStack().top;

            enter_usermode(thread->getStartingAddress(), thread->getUserStack().top);
        }

        void PrepareThread(echis::sched::Thread* thread)
        {
            thread->getParent()->getAddressSpace().switchTo();
            prepare_thread(reinterpret_cast<uint64_t>(UserInit), &thread->getContext(), thread->getKernelStack().top, thread);
            //prepare_thread(thread->getKernelStack().top, thread);
            //thread->getContext()->rip = reinterpret_cast<uint64_t>(UserInit);
            //thread->getContext()->rdi = reinterpret_cast<uint64_t>(thread);
        }

        void SwitchContext(ThreadContext** oldContext, ThreadContext* newContext)
        {
            cpu::core::CoreLocal::Get()->newPml4 = echis::sched::Current()->getParent()->getAddressSpace().pml4;
            switch_context(oldContext, newContext);
        }
    }
}