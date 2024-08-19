#ifndef VIPEROS_ECHIS_SCHED_SCHED_H
#define VIPEROS_ECHIS_SCHED_SCHED_H 1

#include <sched/proc.h>

namespace echis
{
    namespace sched
    {
        void Init();

        void AddProcess(Process&& process);

        void Start();

        void Block();
        void Unblock(Thread* thread);

        void Yield();
    }
}

#endif // VIPEROS_ECHIS_SCHED_SCHED_H