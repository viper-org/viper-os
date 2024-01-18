#ifndef VIPEROS_ECHIS_SCHED_PROCESS_FD_H
#define VIPEROS_ECHIS_SCHED_PROCESS_FD_H 1

#include <sched/pipe.h>

#include <fs/vfs.h>

#include <std/container/ptr.h>

#include <stdint.h>

namespace echis
{
    namespace sched
    {
        namespace OpenMode
        {
            enum OpenMode : uint8_t
            {
                R = 1 << 0,
                W = 1 << 1
            };
        }

        struct FileDescriptor
        {
            fs::vfs::Node* vfsNode;
            vpr::shared_ptr<Pipe> pipe;
            uint8_t flags;
            size_t seek;
        };
    }
}

#endif // VIPEROS_ECHIS_SCHED_PROCESS_FD_H