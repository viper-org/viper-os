#include <sched/Scheduler.h>

#include <drivers/ApicTimer.h>

#include <container/lazy.h>
#include <container/ring.h>

extern "C" void IdleProcess();

namespace sched
{
    constexpr int quantum = 3; // 3 ticks is 30ms

    vpr::lazy<vpr::ring<Process> > processes;
    vpr::lazy<Process> idleProcess;
    int ticks = 0;

    [[noreturn]] void LaunchCurrentProcess()
    {
        ticks = 0;
        processes->current()->launch();
    }

    [[noreturn]] void Schedule()
    {
        if (processes->empty())
        {
            ticks = 0;
            idleProcess->launch();
        }
        processes->next();
        LaunchCurrentProcess();
    }

    void Start()
    {
        idleProcess = Process((uint64_t)&IdleProcess);
        timer::subscribe([](){
            if (++ticks >= quantum)
            {
                Schedule();
            }
        });
        LaunchCurrentProcess();
    }

    void AddProcess(Process p)
    {
        processes->push_back(p);
    }

    void EndCurrentProcess()
    {
        processes->removeCurrent();
        Schedule();
    }

    Process* CurrentProcess()
    {
        return processes->current();
    }
}