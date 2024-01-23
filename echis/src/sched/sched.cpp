#include <sched/sched.h>

#include <atheris/sched/user.h>
#include <atheris/sched/idle.h>

#include <atheris/driver/timer.h>

#include <std/container/ring.h>

namespace echis
{
    namespace sched
    {
        vpr::intrusive_ring<Thread> threads;
        int quantum = 0;
        constexpr int MAX_QUANTUM = 3;
        void Schedule();

        void Start()
        {
            atheris::timer::Subscribe(Schedule);
            atheris::sched::ThreadContext* old; // unused
            atheris::sched::SwitchContext(&old, threads.current()->getContext());
        }

        void AddProcess(Process* process)
        {
            atheris::sched::PrepareThread(process->getMainThread());
            threads.push(process->getMainThread());
        }

        void BlockCurrent()
        {
            Thread* oldThread = threads.current();
            threads.remove();
            if (auto current = threads.current())
            {
                atheris::sched::SwitchContext(&oldThread->getContext(), current->getContext());
            }
            else
            {
                atheris::sched::StartIdleProcess();
            }
        }

        void UnblockThread(Thread* thread)
        {
            threads.push(thread);
        }

        void EndCurrent(int)
        {
            BlockCurrent();
            // TODO(critical): clean up process
        }

        Thread* Current()
        {
            return threads.current();
        }

        void Schedule()
        {
            if (++quantum >= MAX_QUANTUM)
            {
                quantum = 0;
                if (!threads.current())
                {
                    return;
                }

                if (threads.peek() != threads.current())
                {
                    Thread* oldThread = threads.next();
                    atheris::sched::SwitchContext(&oldThread->getContext(), threads.current()->getContext());
                }
            }
        }
    }
}