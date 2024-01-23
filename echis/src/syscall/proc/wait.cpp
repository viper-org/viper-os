#include <syscall/proc/wait.h>

#include <sched/sched.h>

#include <event/exit.h>

namespace echis
{
    namespace syscall
    {
        int wait(int pid)
        {
            event::AddExitHandler(sched::Current(), pid);
            sched::BlockCurrent();
            return pid;
        }
    }
}