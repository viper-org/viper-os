#include <signal/signal.h>

#include <sched/sched.h>

#include <atheris/sched/signal.h>

namespace echis
{
    namespace signal
    {
        void DeliverToCurrent(Signal signal)
        {
            sched::SignalHandler handler = sched::Current()->getSignalHandler(signal);
            atheris::sched::SignalHandler(handler);
        }
    }
}