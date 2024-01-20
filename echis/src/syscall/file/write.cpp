#include <syscall/file/write.h>

#include <event/write.h>

#include <sched/sched.h>

namespace echis
{
    namespace syscall
    {
        size_t write(int fd, const void* buf, size_t count)
        {
            sched::FileDescriptor& file = sched::Current()->getParent()->getFd(fd);

            size_t ret = count;
            if (file.pipe)
            {
                if (file.pipe->getType() != sched::Pipe::Type::Write)
                {
                    return 0;
                }
                sched::WritePipe* writePipe = static_cast<sched::WritePipe*>(file.pipe.get());
                
                ret = writePipe->write(buf, count);
                event::WriteEvent(writePipe);
                return ret;
            }

            file.vfsNode->write(buf, count, file.seek);
            event::WriteEvent(file.vfsNode);
            return ret;
        }
    }
}