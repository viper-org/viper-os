#include <sched/sched.h>

#include <atheris/driver/timer.h>

#include <atheris/sched/sched.h>

#include <atheris/cpu/int.h>

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

        void Schedule();

        void Init()
        {
            processList.default_init();
            threadList.default_init();
        }

        void AddProcess(Process&& process)
        {
            processList->push_back(process);
            threadList->push(processList->back().getMainThread());
            atheris::sched::PrepareThread(processList->back().getMainThread());
        }

        void Start()
        {
            atheris::cpu::DisableInt();
            atheris::timer::Subscribe(Schedule); // TODO: Proper event system that we subscribe to

            atheris::sched::ThreadContext* old; // unused
            atheris::sched::SwitchContext(&old, threadList->current()->getContext());
        }


        Thread* Current()
        {
            return threadList->current();
        }
        
        void Block()
        {
            auto current = threadList->current();
            threadList->remove();

            // TODO: Go to idle thread if no others found
            if (!threadList->current())
            {
                asm("1: hlt; jmp 1b");
            }
            atheris::cpu::PushDisableInt();
            atheris::sched::SwitchContext(&current->getContext(), threadList->current()->getContext());
            atheris::cpu::PopDisableInt();
        }

        void Unblock(Thread* thread)
        {
            threadList->push(thread);
        }

        void Yield()
        {
            if (!threadList->current()) return;
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