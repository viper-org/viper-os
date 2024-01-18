#include <syscall/file/seek.h>

#include <sched/sched.h>

namespace echis
{
    namespace syscall
    {
        size_t seek(int fd, size_t offset)
        {
            sched::FileDescriptor& file = sched::Current()->getParent()->getFd(fd);
            file.seek = offset;
            
            return offset;
        }
    }
}