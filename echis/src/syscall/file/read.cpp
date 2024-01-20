#include <syscall/file/read.h>

#include <sched/sched.h>

namespace echis
{
    namespace syscall
    {
        size_t read(int fd, void* buf, size_t count)
        {
            sched::FileDescriptor& file = sched::Current()->getParent()->getFd(fd);

            size_t ret = count;
            if (file.pipe)
            {
                if (file.pipe->getType() != sched::Pipe::Type::Read)
                {
                    return 0;
                }
                sched::ReadPipe* readPipe = static_cast<sched::ReadPipe*>(file.pipe.get());
                
                ret = readPipe->read(buf, count);
            }
            else
            {
                file.vfsNode->read(buf, &count, file.seek);
            }

            return ret;
        }
    }
}