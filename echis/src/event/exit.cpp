#include <event/exit.h>

#include <sched/process.h>

#include <fs/vfs.h>

#include <std/container/late_init.h>
#include <std/container/vector.h>
#include <std/thread/mutex.h>

namespace echis
{
    namespace event
    {
        struct ExitEventObject
        {
            sched::Thread* thread;
            int pid;
        };

        vpr::late_init<vpr::vector<ExitEventObject>> exitEventWaiting;
        vpr::mutex exitEventMutex;

        void ExitEvent(int pid)
        {
            for (auto waiting : *exitEventWaiting)
            {
                if (waiting.pid == pid)
                {
                    waiting.thread->unblock();

                    waiting.pid = -1;
                    waiting.thread = nullptr;
                }
            }
        }

        void AddExitHandler(sched::Thread* thread, int pid)
        {
            vpr::lock_guard<vpr::mutex> lock(exitEventMutex);

            for (auto& waiting : *exitEventWaiting)
            {
                if (waiting.thread == nullptr) // Free slot for a new waiting object
                {
                    waiting.thread = thread;
                    waiting.pid = pid;
                }
            }
            
            exitEventWaiting->push_back(ExitEventObject{thread, pid});
        }
    }
}