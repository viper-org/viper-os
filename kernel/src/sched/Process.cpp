#include <sched/Process.h>
#include <sched/Scheduler.h>

#include <mm/Paging.h>
#include <mm/Pmm.h>

extern "C" [[noreturn]] void launch_kernel_task(cpu::Context* context);

namespace sched
{
    Thread::Thread(Process* parent, uint64_t start) 
        : mParent(parent)
        , next(nullptr)
        , previous(nullptr)
    {
        mContext.baseFrame.rip = start;
        mContext.baseFrame.cs = 0x8;
        mContext.baseFrame.ss = 0x10;
        mContext.baseFrame.rflags = 0x246;
        mContext.baseFrame.rsp = PMM::PhysToVirt(PMM::GetPage()); // TODO: Replace ASAP!!

        mContext.controlRegisters.cr4 = 0x20;
        mContext.controlRegisters.cr3 = (uint64_t)paging::KernelAddrSpace()->pml4;
        mContext.controlRegisters.cr0 = 0x80010011;
    }

    void Thread::launch()
    {
        launch_kernel_task(&mContext);
    }

    void Thread::terminate()
    {
        if (mParent->getMainThread().get() == this) // Terminating main thread - terminate the process
        {
            sched::RemoveThreadsByPid(mParent->getPid());

            vpr::shared_ptr<Thread> current = *mParent->getThreads().begin();
            vpr::shared_ptr<Thread> next;

            // Get rid of references of the threads so they can be freed
            for (; current != *mParent->getThreads().end(); current = next)
            {
                next = current->next;
                current->next = nullptr;
                current->previous = nullptr;
            }
            mParent->getThreads().clear();
        }
    }

    cpu::Context& Thread::getContext()
    {
        return mContext;
    }

    Process* Thread::getParent()
    {
        return mParent;
    }


    static int pid = 0;

    Process::Process()
        : mMainThread(vpr::make_shared<Thread>(this, 0))
        , mPid(pid++)
    {
        mThreads.push_front(mMainThread);
    }

    Process::Process(uint64_t start)
        : mMainThread(vpr::make_shared<Thread>(this, start))
        , mPid(pid++)
    {
        mThreads.push_front(mMainThread);

        mPrivilege = Privilege::Kernel;
    }


    vpr::shared_ptr<Thread>& Process::getMainThread()
    {
        return mMainThread;
    }

    int Process::getPid() const
    {
        return mPid;
    }
}