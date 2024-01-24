#include <syscall/file/movfd.h>

#include <sched/sched.h>

namespace echis
{
    namespace syscall
    {
        int movfd(int oldfd, int newfd)
        {
            return sched::Current()->getParent()->moveFileDescription(oldfd, newfd);
        }
    }
}