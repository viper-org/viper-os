#include <sched/sched.h>

#include <atheris/sched/sched.h>

#include <intrusive_ring.h>
#include <lazy_init.h>
#include <vector>

namespace echis
{
    namespace sched
    {
        static lazy_init<std::vector<Process> > processList;
        static lazy_init<intrusive_ring<Thread> > threadList;

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
            atheris::sched::ThreadContext* old; // unused
            atheris::sched::SwitchContext(&old, threadList->current()->getContext());
        }

        
        void Yield()
        {
            if (threadList->peek() == threadList->current()) return;

            auto old = threadList->next();
            atheris::sched::SwitchContext(&old->getContext(), threadList->current()->getContext());
        }
    }
}