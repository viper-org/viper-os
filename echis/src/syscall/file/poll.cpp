#include "sched/pipe.h"
#include <syscall/file/poll.h>

#include <event/write.h>

#include <sched/sched.h>

namespace echis
{
    namespace syscall
    {
        int poll(int fd)
        {
            sched::FileDescriptor& file = sched::Current()->getParent()->getFd(fd);
            if (file.pipe)
            {
                if (file.pipe->getType() != sched::Pipe::Type::Read)
                {
                    return -1;
                }
                sched::ReadPipe* readPipe = static_cast<sched::ReadPipe*>(file.pipe.get());
                if (!readPipe->getWriteEnd()->empty())
                {
                    return 0; // Already data to read
                }

                event::AddWriteHandler(sched::Current(), file.pipe.get());
            }
            else
            {
                event::AddWriteHandler(sched::Current(), file.vfsNode);
            }
            sched::BlockCurrent();
            return 0;
        }
    }
}