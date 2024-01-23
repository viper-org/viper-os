#include <syscall/proc/raise.h>

#include <signal/signal.h>

namespace echis
{
    namespace syscall
    {
        int raise(int sig)
        {
            if (sig > 32 || sig < 0)
            {
                return 1;
            }

            signal::DeliverToCurrent(static_cast<signal::Signal>(sig));
            return 0;
        }
    }
}