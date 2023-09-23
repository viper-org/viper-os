#include <sched/Scheduler.h>

#include <drivers/ApicTimer.h>

#include <container/atomic.h>
#include <container/lazy.h>
#include <container/ring.h>

extern "C" void IdleProcess();

namespace sched
{
    constexpr int quantum = 3; // 3 ticks is 30ms
    vpr::atomic_int ticks = 0;

    vpr::lazy<Process::ThreadList> readyThreads;

     void LaunchCurrentThread()
    {
        ticks = 0;
        (*readyThreads->begin())->launch();
    }

    void CycleCurrent()
    {
        vpr::shared_ptr<Thread> current = *readyThreads->begin();
        readyThreads->erase(readyThreads->begin());
        readyThreads->push_back(current);
    }

    void Schedule()
    {
        CycleCurrent();
        LaunchCurrentThread();
    }

    void Start()
    {
        timer::subscribe([](){
            if (++ticks >= quantum)
            {
                Schedule();
            }
        });
        LaunchCurrentThread();
    }

    void AddProcess(const Process& p)
    {
        for (const auto& thread : p.getThreads())
        {
            readyThreads->push_back(thread);
        }
    }

    Thread* CurrentThread()
    {
        return readyThreads->begin()->get();
    }
}