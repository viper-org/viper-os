#include <syscall/file/read.h>

#include <sched/sched.h>

namespace echis
{
    namespace syscall
    {
        size_t read(int fd, void* buf, size_t count)
        {
            sched::FileDescriptor& file = sched::Current()->getParent()->getFd(fd);

            file.vfsNode->read(buf, &count);
            return count;
        }
    }
}