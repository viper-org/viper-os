#include <sched/sched.h>

#include <atheris/driver/timer.h>

#include <atheris/sched/sched.h>

#include <algorithm>
#include <intrusive_ring.h>
#include <lazy_init.h>
#include <vector>

namespace echis
{
    namespace sched
    {
        static lazy_init<std::vector<Process> > processList;
        static lazy_init<intrusive_ring<Thread> > threadList;
        static lazy_init<std::vector<Thread*> > blockedThreads;

        void Schedule();

        void Init()
        {
            processList.default_init();
            threadList.default_init();
            blockedThreads.default_init();
        }

        void AddProcess(Process&& process)
        {
            processList->push_back(process);
            threadList->push(processList->back().getMainThread());
            atheris::sched::PrepareThread(processList->back().getMainThread());
        }

        void Start()
        {
            atheris::timer::Subscribe(Schedule); // TODO: Proper event system that we subscribe to

            atheris::sched::ThreadContext* old; // unused
            atheris::sched::SwitchContext(&old, threadList->current()->getContext());
        }

        
        void Block()
        {
            auto current = threadList->current();
            threadList->remove();
            blockedThreads->push_back(current);

            // TODO: Go to idle thread if no others found
            atheris::sched::SwitchContext(&current->getContext(), threadList->current()->getContext());
        }

        void Unblock(Thread* thread)
        {
            auto it = std::find(blockedThreads->begin(), blockedThreads->end(), thread);
            if (it == blockedThreads->end()) return; // probably error here

            threadList->push(*it);
            blockedThreads->erase(it);
        }

        void Yield()
        {
            if (threadList->peek() == threadList->current()) return;

            auto old = threadList->next();
            atheris::sched::SwitchContext(&old->getContext(), threadList->current()->getContext());
        }

        constexpr int quantum = 3;
        static int currentTimestep = 0;
        
        void Schedule()
        {
            ++currentTimestep;
            if (currentTimestep == quantum)
            {
                currentTimestep = 0;
                Yield();
            }
            // TODO: Check events
        }
    }
}