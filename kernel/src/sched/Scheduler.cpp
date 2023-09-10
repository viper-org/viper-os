#include <sched/Scheduler.h>

#include <drivers/ApicTimer.h>

#include <container/lazy.h>
#include <container/ring.h>

namespace sched
{
    constexpr int quantum = 3; // 3 ticks is 30ms

    vpr::lazy<vpr::ring<Process> > processes;
    int ticks = 0;

    void Start()
    {
        timer::subscribe([](){
            if (++ticks >= quantum)
            {
                ticks = 0;
                processes->next();
                processes->current()->launch();
            }
        });
        processes->current()->launch();
    }

    void AddProcess(Process p)
    {
        processes->push_back(p);
    }

    Process* CurrentProcess()
    {
        return processes->current();
    }
}