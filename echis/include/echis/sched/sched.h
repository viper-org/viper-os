#ifndef VIPEROS_ECHIS_SCHED_SCHED_H
#define VIPEROS_ECHIS_SCHED_SCHED_H 1

#include <sched/process.h>

namespace echis
{
    namespace sched
    {
        void Start();

        void AddProcess(Process* process);

        Thread* Current();
    }
}

#endif // VIPEROS_ECHIS_SCHED_SCHED_H