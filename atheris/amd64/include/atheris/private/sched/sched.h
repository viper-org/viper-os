#ifndef VIPEROS_ATHERIS_X64_ATHERIS_SCHED_SCHED_H
#define VIPEROS_ATHERIS_X64_ATHERIS_SCHED_SCHED_H 1

#include <stdint.h>

namespace echis::sched
{
    class Thread;
}

namespace atheris
{
    namespace sched
    {
        struct [[gnu::packed]] ThreadContext
        {
            uint64_t rbp;
            uint64_t rbx;
            uint64_t rdi;
            uint64_t rsi;
            uint64_t r12;
            uint64_t r13;
            uint64_t r14;
            uint64_t r15;
            uint64_t rflags;
            uint64_t rip;
        };

        void PrepareThread(echis::sched::Thread* thread);
        void SwitchContext(ThreadContext** oldContext, ThreadContext* newContext);
    }
}

#endif // VIPEROS_ATHERIS_X64_ATHERIS_SCHED_SCHED_H