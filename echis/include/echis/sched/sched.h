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
    }
}

#endif // VIPEROS_ECHIS_SCHED_SCHED_H