#ifndef VIPER_OS_SCHED_PROCESS_H
#define VIPER_OS_SCHED_PROCESS_H 1

#include <cpu/Context.h>

#include <container/ptr.h>
#include <container/list.h>


namespace sched
{
    class Process;

    class Thread
    {
    friend class Process;
    public:
        Thread(Process* parent, uint64_t start);

        [[noreturn]] void launch();

        void terminate();

        cpu::Context& getContext();
        Process* getParent();

    private:
        cpu::Context mContext;
        Process* mParent;

        vpr::shared_ptr<Thread> next;
        vpr::shared_ptr<Thread> previous;
    };

    class Process
    {
    friend class Thread;
    public:
        using ThreadList = vpr::intrusive_list<vpr::shared_ptr<Thread>,
        [](vpr::shared_ptr<Thread>& thread) -> vpr::shared_ptr<Thread>& {
            return thread->next;
        },
        [](vpr::shared_ptr<Thread>& thread) -> vpr::shared_ptr<Thread>& {
            return thread->previous;
        }>;

        enum class Privilege
        {
            User,
            Kernel
        };

        Process();

        Process(uint64_t start);

        vpr::shared_ptr<Thread>& getMainThread();

        ThreadList& getThreads()
        {
            return mThreads;
        }

        int getPid() const;

    private:
        ThreadList mThreads;
        vpr::shared_ptr<Thread> mMainThread;

        Privilege mPrivilege;

        int mPid;
    };
}

#endif