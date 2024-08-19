#include <sched/proc.h>

#include <mm/physical.h>
#include <mm/vm/alloc.h>

namespace echis
{
    namespace sched
    {
        std::uint64_t& Thread::getExecStart()
        {
            return mExecStart;
        }

        const ThreadStack& Thread::getKernelStack() const
        {
            return mKernelStack;
        }

        atheris::sched::ThreadContext*& Thread::getContext()
        {
            return mContext;
        }

        Thread::Thread(Process* process, tid_t tid)
            : mProcess(process)
            , mKernelStack(0, 0)
            , mExecStart(0)
            , mTid(tid)
        {
        }

        void Thread::allocateStack(std::uint64_t stackSize)
        {
            auto bottom = vm::alloc::GetKernelPages(
                stackSize / mm::physical::GetPageSize(),
                atheris::vm::flags::present | atheris::vm::flags::write);

            mKernelStack.bottom = reinterpret_cast<std::uint64_t>(bottom);
            mKernelStack.top = mKernelStack.bottom + stackSize;
        }

        static pid_t nextPid = 0;
        Process::Process()
            : nextTid(0)
            , mMainThread(this, nextTid++)
            , mAddressSpace(atheris::vm::AddressSpace::Create())
            , mPid(nextPid++)
        {
            mMainThread.allocateStack(0x4000);
        }

        Thread* Process::getMainThread()
        {
            return &mMainThread;
        }
    }
}