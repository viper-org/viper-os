#include <atheris/sched/sched.h>

#include <atheris/mm/vm.h>

#include <echis/sched/proc.h>

#include <new>

namespace atheris
{
    namespace sched
    {
        extern "C" void prepare_thread(uint64_t func, ThreadContext** context, uint64_t kernelStack, echis::sched::Thread* thread);
        extern "C" void switch_context(ThreadContext** oldContext, ThreadContext* newContext);

        void PrepareThread(echis::sched::Thread* thread)
        {
            void* contextLoc = reinterpret_cast<void*>(thread->getKernelStack().top - sizeof(ThreadContext));
            thread->getContext() = new (contextLoc) ThreadContext;
            thread->getContext()->rip = thread->getExecStart();
            thread->getContext()->rflags = 0x202;
        }

        void SwitchContext(ThreadContext** oldContext, ThreadContext* newContext)
        {
            switch_context(oldContext, newContext);
        }
    }
}