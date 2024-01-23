#ifndef VIPEROS_ECHIS_SIGNAL_SIGNAL_H
#define VIPEROS_ECHIS_SIGNAL_SIGNAL_H 1

namespace echis
{
    namespace signal
    {
        enum Signal
        {
            NONE=-1,
            SIGUSR1=10,
            SIGSEGV=11,
        };

        struct PendingSignalObject
        {
            Signal signum;
        };

        void DeliverToCurrent(Signal signal);
        PendingSignalObject* CheckIncoming();

        void(*GetHandler(PendingSignalObject*))(int);
    }
}

#endif // VIPEROS_ECHIS_SIGNAL_SIGNAL_H