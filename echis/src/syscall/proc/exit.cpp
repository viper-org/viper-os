#include <syscall/proc/exit.h>

#include <sched/sched.h>

#include <event/exit.h>

namespace echis
{
    namespace syscall
    {
        void exit(int status)
        {
            event::ExitEvent(sched::Current()->getParent()->getPid());
            sched::EndCurrent(status);
        }
    }
}