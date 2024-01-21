#include <syscall/proc/sigaction.h>

#include <sched/sched.h>

namespace echis
{
    namespace syscall
    {
        int sigaction(int signum, SignalHandler handler)
        {
            sched::Current()->getSignalHandler(signum) = handler;
            return 0;
        }
    }
}