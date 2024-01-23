#include <syscall/proc/exit.h>

#include <sched/sched.h>

namespace echis
{
    namespace syscall
    {
        void exit(int status)
        {
            sched::EndCurrent(status);
        }
    }
}