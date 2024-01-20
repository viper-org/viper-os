#include <event/write.h>

#include <sched/process.h>

#include <fs/vfs.h>

#include <std/container/late_init.h>
#include <std/container/vector.h>
#include <std/thread/mutex.h>

namespace echis
{
    namespace event
    {
        struct WriteEventObject
        {
            sched::Thread* thread;
            union
            {
                fs::vfs::Node* vfsNode;
                sched::Pipe* pipe;
            };
        };

        vpr::late_init<vpr::vector<WriteEventObject>> waiting;
        vpr::mutex mutex;

        void WriteEvent(fs::vfs::Node* vfsNode)
        {
            for (auto waiting : *waiting)
            {
                if (waiting.vfsNode == vfsNode)
                {
                    waiting.thread->unblock();

                    waiting.thread = nullptr;
                    waiting.pipe = nullptr;
                }
            }
        }

        void WriteEvent(sched::Pipe* pipe)
        {
            for (auto waiting : *waiting)
            {
                if (waiting.pipe == pipe)
                {
                    waiting.thread->unblock();

                    waiting.thread = nullptr;
                    waiting.pipe = nullptr;
                }
            }
        }

        void AddWriteHandler(sched::Thread* thread, fs::vfs::Node* vfsNode)
        {
            vpr::lock_guard<vpr::mutex> lock(mutex);

            for (auto& waiting : *waiting)
            {
                if (waiting.thread == nullptr) // Free slot for a new waiting object
                {
                    waiting.thread = thread;
                    waiting.vfsNode = vfsNode;
                }
            }
            
            waiting->push_back(WriteEventObject{thread, {vfsNode}});
        }

        void AddWriteHandler(sched::Thread* thread, sched::Pipe* pipe)
        {
            vpr::lock_guard<vpr::mutex> lock(mutex);
            sched::ReadPipe* readPipe = static_cast<sched::ReadPipe*>(pipe);

            for (auto& waiting : *waiting)
            {
                if (waiting.thread == nullptr) // Free slot for a new waiting object
                {
                    waiting.thread = thread;
                    waiting.pipe = readPipe->getWriteEnd();
                }
            }

            waiting->push_back(WriteEventObject{thread, {.pipe = readPipe->getWriteEnd()}});
        }
    }
}