#ifndef VIPEROS_ECHIS_EVENT_EXIT_H
#define VIPEROS_ECHIS_EVENT_EXIT_H 1

namespace echis
{
    namespace sched
    {
        class Thread;
        class Process;
    }

    namespace event
    {
        void ExitEvent(int pid);

        void AddExitHandler(sched::Thread* thread, int pid);
    }
}

#endif // VIPEROS_ECHIS_EVENT_EXIT_H