#include <syscall/file/write.h>

#include <sched/sched.h>

namespace echis
{
    namespace syscall
    {
        size_t write(int fd, const void* buf, size_t count)
        {
            sched::FileDescriptor& file = sched::Current()->getParent()->getFd(fd);

            if (file.pipe)
            {
                if (file.pipe->getType() != sched::Pipe::Type::Write)
                {
                    return 0;
                }
                sched::WritePipe* writePipe = static_cast<sched::WritePipe*>(file.pipe);
                
                return writePipe->write(buf, count);
            }

            if (file.vfsNode->write(buf, count, file.seek) == -1)
            {
                return 0;
            }
            return count;
        }
    }
}