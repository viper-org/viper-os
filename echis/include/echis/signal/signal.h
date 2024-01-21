#ifndef VIPEROS_ECHIS_SIGNAL_SIGNAL_H
#define VIPEROS_ECHIS_SIGNAL_SIGNAL_H 1

namespace echis
{
    namespace signal
    {
        enum Signal
        {
            SIGSEGV=11
        };

        void DeliverToCurrent(Signal signal);
    }
}

#endif // VIPEROS_ECHIS_SIGNAL_SIGNAL_H