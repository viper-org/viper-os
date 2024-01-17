#include <syscall/file/close.h>

#include <sched/sched.h>

namespace echis
{
    namespace syscall
    {
        int close(int fd)
        {
            return sched::Current()->getParent()->closeFileDescription(fd);
        }
    }
}