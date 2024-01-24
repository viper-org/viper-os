#ifndef VIPEROS_ECHIS_SIGNAL_SIGNAL_H
#define VIPEROS_ECHIS_SIGNAL_SIGNAL_H 1

namespace echis
{
    namespace signal
    {
        enum Signal
        {
            NONE=-1,
            SIGHUP=1,
            SIGINT=2,
            SIGQUIT=3,
            SIGILL=4,
            SIGTRAP=5,
            SIGABRT=6,
            SIGIOT=6,
            SIGBUS=7,
            SIGFPE=8,
            SIGKILL=9,
            SIGUSR1=10,
            SIGSEGV=11,
            SIGUSR2=12,
            SIGPIPE=13,
            SIGALRM=14,
            SIGTERM=15,
            SIGSTKFLT=16,
            SIGCHLD=17,
            SIGCONT=18,
            SIGSTOP=19,
            SIGTSTP=20,
            SIGTTIN=21,
            SIGTTOU=22,
            SIGURG=23,
            SIGXCPU=24,
            SIGXFSZ=25,
            SIGVTALRM=26,
            SIGPROF=27,
            SIGWINCH=28,
            SIGIO=29,
            SIGPWR=30,
            SIGSYS=31,
            SIGUNUSED=31
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