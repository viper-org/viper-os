#include "sched/sched.h"
#include <atheris/private/sched/signal.h>

namespace atheris
{
    namespace sched
    {
        extern "C" void enter_signal_handler(void(*handler)(int), uint64_t stack);

        void SignalHandler(void(*handler)(int))
        {
            enter_signal_handler(handler, echis::sched::Current()->getUserStack().top);
        }
    }
}