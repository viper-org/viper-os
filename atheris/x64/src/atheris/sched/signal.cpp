#include <atheris/private/sched/signal.h>

#include <echis/sched/sched.h>

namespace atheris
{
    namespace sched
    {
        extern "C" void enter_signal_handler(SavedUserContext*, void(*handler)(int), uint64_t stack);
        extern "C" void return_from_signal_handler(uint64_t rsp);

        void SignalHandler(SavedUserContext context, void(*handler)(int))
        {
            enter_signal_handler(&context, handler, echis::sched::Current()->getSignalHandlerStack().top);
        }
        
        void ReturnFromSignalHandler(uint64_t rsp)
        {
            return_from_signal_handler(rsp);
        }
    }
}