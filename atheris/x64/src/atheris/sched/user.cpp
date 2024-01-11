#include <atheris/private/sched/user.h>

#include <cpu/gdt/tss.h>

#include <echis/sched/process.h>

namespace atheris
{
    namespace sched
    {
        extern "C" void enter_usermode(uint64_t rip, uint64_t rsp);

        void UserInit(echis::sched::Thread* thread)
        {
            thread->getParent()->getAddressSpace().switchTo();
            x64::cpu::tss::SetRSP0(reinterpret_cast<uint64_t>(thread->getKernelStack().top));

            enter_usermode(thread->getStartingAddress(), 0);
        }
    }
}