#ifndef VIPEROS_ECHIS_SCHED_PROCESS_H
#define VIPEROS_ECHIS_SCHED_PROCESS_H 1

#include <sched/processFd.h>
#include <sched/pipe.h>

#include <signal/signal.h>

#include <atheris/mm/vm.h>

#include <std/container/vector.h>

#include <stddef.h>

namespace atheris::sched
{
    struct ThreadContext;
}

namespace echis
{
    namespace sched
    {
        constexpr int MAX_FD = 16;

        class Process;

        using SignalHandler = void(*)(int);

        class Thread
        {
        public:
            Thread();
            Thread(Process& parent, uint64_t start);

            struct Stack
            {
                uint64_t top;
                size_t   size;
            };

            Process* getParent() const;
            uint64_t getStartingAddress() const;
            void setStartingAddress(uint64_t newStart);
            Stack& getUserStack();
            Stack& getSignalHandlerStack();
            Stack& getKernelStack();
            atheris::sched::ThreadContext*& getContext();

            SignalHandler& getSignalHandler(int signum);
            vpr::vector<signal::PendingSignalObject>& getPendingSignals();

            void unblock();

            Thread* next; // For intrusive circular linked list in scheduler

        private:
            Process* mParent;
            Stack mUserStack;
            Stack mSignalHandlerStack;
            Stack mKernelStack;
            atheris::sched::ThreadContext* mContext;
            uint64_t mStart;

            SignalHandler mSignalHandlers[32];
            vpr::vector<signal::PendingSignalObject> mPendingSignals;

            int mWaitingFd;
        };

        class Process
        {
        public:
            Process();
            Process(int pid, uint64_t startAddress);

            int getPid() const;
            Thread* getMainThread();
            atheris::vm::AddressSpace& getAddressSpace();
            FileDescriptor& getFd(int n);

            void copyFileDescriptionsFrom(Process* other);
            int addOpenFileDescription(fs::vfs::Node* node, OpenMode::OpenMode mode);
            int addOpenFileDescription(vpr::shared_ptr<Pipe> pipe);
            int closeFileDescription(int fd);

            static Process* Create(const char* path);

        private:
            int mPid;
            Thread mMainThread;
            atheris::vm::AddressSpace mAddressSpace;
            FileDescriptor mFds[MAX_FD];
        };
    }
}

#endif // VIPEROS_ECHIS_SCHED_PROCESS_H