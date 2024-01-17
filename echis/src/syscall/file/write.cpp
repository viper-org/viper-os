#include <syscall/file/write.h>

#include <sched/sched.h>

namespace echis
{
    namespace syscall
    {
        size_t write(int fd, const void* buf, size_t count)
        {
            sched::FileDescriptor& file = sched::Current()->getParent()->getFd(fd);

            if (file.vfsNode->write(buf, count) == -1)
            {
                return 0;
            }
            return count;
        }
    }
}