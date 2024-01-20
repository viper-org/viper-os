#ifndef VIPEROS_ECHIS_EVENT_WRITE_H
#define VIPEROS_ECHIS_EVENT_WRITE_H 1

namespace echis
{
    namespace sched
    {
        class Thread;
        class Pipe;
    }

    namespace fs::vfs
    {
        class Node;
    }

    namespace event
    {
        void WriteEvent(fs::vfs::Node* vfsNode);
        void WriteEvent(sched::Pipe* pipe);

        void AddWriteHandler(sched::Thread*, fs::vfs::Node* vfsNode);
        void AddWriteHandler(sched::Thread*, sched::Pipe* pipe);
    }
}

#endif // VIPEROS_ECHIS_EVENT_WRITE_H