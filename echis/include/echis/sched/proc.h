#include "atheris/private/sched/sched.h"
#ifndef VIPEROS_ECHIS_SCHED_PROC_H
#define VIPEROS_ECHIS_SCHED_PROC_H 1

#include <atheris/mm/vm.h>

#include <atheris/sched/sched.h>

#include <cstdint>

namespace echis
{
    namespace sched
    {
        class Process;

        struct ThreadStack
        {
            std::uint64_t bottom;
            std::uint64_t top;
        };

        using tid_t = unsigned int;
        class Thread
        {
        friend class Process;
        public:
            std::uint64_t& getExecStart();
            const ThreadStack& getKernelStack() const;
            atheris::sched::ThreadContext*& getContext();

            Thread* next; // for scheduler

        private:
            Thread(Process* process, tid_t tid);
            void allocateStack(std::uint64_t stackSize);

            Process* mProcess;

            ThreadStack mKernelStack;
            // TODO: Add user stack
            std::uint64_t mExecStart;

            atheris::sched::ThreadContext* mContext;

            tid_t mTid;
        };

        using pid_t = unsigned int;
        class Process
        {
        public:
            Process();

            Thread* getMainThread();

        private:
            tid_t nextTid;
            Thread mMainThread;

            atheris::vm::AddressSpace mAddressSpace;

            pid_t mPid;
        };
    }
}

#endif // VIPEROS_ECHIS_SCHED_PROC_H