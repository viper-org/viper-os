#include <sched/Process.h>

#include <mm/Paging.h>

extern "C" [[noreturn]] void launch_kernel_task(cpu::Context* context);

namespace sched
{
    Process::Process()
    {
    }

    Process::Process(uint64_t start)
    {
        mContext.baseFrame.rip = start;
        mContext.baseFrame.cs = 0x8;
        mContext.baseFrame.ss = 0x10;
        mContext.baseFrame.rflags = 0x246;
        
        mContext.controlRegisters.cr4 = 0x20;
        mContext.controlRegisters.cr3 = (uint64_t)Paging::KernelAddrSpace()->pml4;
        mContext.controlRegisters.cr0 = 0x80010011;

        mPrivilege = Privilege::Kernel;
    }


    void Process::launch()
    {
        switch (mPrivilege)
        {
            case Privilege::Kernel:
                launch_kernel_task(&mContext);
            case Privilege::User:
                launch_kernel_task(&mContext);
        }
    }

    cpu::Context* Process::getContext()
    {
        return &mContext;
    }
}