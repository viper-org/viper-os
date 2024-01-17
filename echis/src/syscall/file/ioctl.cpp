#include <syscall/file/ioctl.h>

#include <sched/sched.h>

namespace echis
{
    namespace syscall
    {
        int ioctl(int fd, unsigned long request, char* arg)
        {
            sched::FileDescriptor& file = sched::Current()->getParent()->getFd(fd);

            return file.vfsNode->ioctl(request, arg);
        }
    }
}