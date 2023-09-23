#ifndef VIPER_OS_SCHED_SCHEDULER_H
#define VIPER_OS_SCHED_SCHEDULER_H 1

#include <sched/Process.h>

namespace sched
{
    void Start();

    void AddProcess(const Process& p);

    Thread* CurrentThread();
}

#endif