#include <sched/process.h>

namespace echis
{
    namespace sched
    {
        Thread::Thread()
        {
        }

        Thread::Thread(Process& parent, uint64_t start)
            : mParent(&parent)
            , mStart(start)
        {
        }

        Process* Thread::getParent() const
        {
            return mParent;
        }

        uint64_t Thread::getStartingAddress() const
        {
            return mStart;
        }

        Thread::Stack& Thread::getUserStack()
        {
            return mUserStack;
        }

        Thread::Stack& Thread::getKernelStack()
        {
            return mKernelStack;
        }


        Process::Process()
        {
        }

        Process::Process(uint64_t startAddress)
            : mMainThread(Thread(*this, startAddress))
            , mAddressSpace(atheris::vm::AddressSpace::Create())
        {
        }

        int Process::getPid() const
        {
            return mPid;
        }

        Thread* Process::getMainThread()
        {
            return &mMainThread;
        }
        
        atheris::vm::AddressSpace& Process::getAddressSpace()
        {
            return mAddressSpace;
        }
    }
}