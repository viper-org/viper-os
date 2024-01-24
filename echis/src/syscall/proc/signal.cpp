#include <syscall/proc/signal.h>

#include <sched/sched.h>

namespace echis
{
    namespace syscall
    {
        SignalHandler signal(int signum, SignalHandler handler)
        {
            SignalHandler oldHandler = sched::Current()->getSignalHandler(signum);
            sched::Current()->getSignalHandler(signum) = handler;
            return oldHandler;
        }
    }
}