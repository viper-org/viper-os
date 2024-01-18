#include <syscall/proc/pipe.h>

#include <sched/pipe.h>

namespace echis
{
    namespace syscall
    {
        int pipe(int fds[2])
        {
            sched::CreatePipe(fds);
            return 0;
        }
    }
}