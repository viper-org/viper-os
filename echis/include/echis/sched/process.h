#ifndef VIPEROS_ECHIS_SCHED_PROCESS_H
#define VIPEROS_ECHIS_SCHED_PROCESS_H 1

#include <atheris/mm/vm.h>

#include <stddef.h>

namespace echis
{
    namespace sched
    {
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
            Stack& getUserStack();
            Stack& getKernelStack();

            Thread* next; // For intrusive circular linked list in scheduler

        private:
            Process* mParent;
            Stack mUserStack;
            Stack mKernelStack;
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

        private:
            int mPid;
            Thread mMainThread;
            atheris::vm::AddressSpace mAddressSpace;
        };
    }
}

#endif // VIPEROS_ECHIS_SCHED_PROCESS_H