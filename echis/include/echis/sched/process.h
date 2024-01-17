#ifndef VIPEROS_ECHIS_SCHED_PROCESS_H
#define VIPEROS_ECHIS_SCHED_PROCESS_H 1

#include <sched/processFd.h>

#include <atheris/mm/vm.h>

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
            Stack& getKernelStack();
            atheris::sched::ThreadContext*& getContext();

            Thread* next; // For intrusive circular linked list in scheduler

        private:
            Process* mParent;
            Stack mUserStack;
            Stack mKernelStack;
            atheris::sched::ThreadContext* mContext;
            uint64_t mStart;
        };

        class Process
        {
        public:
            Process();
            Process(uint64_t startAddress);

            int getPid() const;
            Thread* getMainThread();
            atheris::vm::AddressSpace& getAddressSpace();
            FileDescriptor& getFd(int n);

            int addOpenFileDescription(fs::vfs::Node* node, OpenMode::OpenMode mode);

        private:
            int mPid;
            Thread mMainThread;
            atheris::vm::AddressSpace mAddressSpace;
            FileDescriptor mFds[MAX_FD];
        };
    }
}

#endif // VIPEROS_ECHIS_SCHED_PROCESS_H