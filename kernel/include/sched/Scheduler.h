#ifndef VIPER_OS_SCHED_SCHEDULER_H
#define VIPER_OS_SCHED_SCHEDULER_H 1

#include <sched/Process.h>

namespace sched
{
    void Start();

    void AddProcess(Process& p);

    Thread* CurrentThread();

    void RemoveCurrentThread();
    void RemoveThreadsByPid(int pid);
}

#endif