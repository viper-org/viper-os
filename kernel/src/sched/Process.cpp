#include <sched/Process.h>

#include <mm/Paging.h>
#include <mm/Pmm.h>

extern "C" [[noreturn]] void launch_kernel_task(cpu::Context* context);

namespace sched
{
    Thread::Thread(Process* parent, uint64_t start) 
        : mParent(parent)
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

    cpu::Context& Thread::getContext()
    {
        return mContext;
    }

    Process::Process()
        : mMainThread(vpr::make_shared<Thread>(this, 0))
    {
        mThreads.push_front(mMainThread);
    }

    Process::Process(uint64_t start)
        : mMainThread(vpr::make_shared<Thread>(this, start))
    {
        mThreads.push_front(mMainThread);

        mPrivilege = Privilege::Kernel;
    }


    vpr::shared_ptr<Thread>& Process::getMainThread()
    {
        return mMainThread;
    }
}