#include <mm/pmm.h>
#include <mm/heap.h>
#include <mm/vmm.h>

#include <sched/process.h>
#include <sched/sched.h>

#include <fs/tmpfs.h>
#include <fs/initrd.h>

#include <driver/console.h>

#include <atheris/driver/framebuffer.h>
#include <atheris/driver/console.h>
#include <atheris/driver/timer.h>

#include <atheris/mm/pm.h>
#include <atheris/mm/vm.h>

#include <atheris/cpu/smp.h>
#include <atheris/cpu/core.h>
#include <atheris/cpu/halt.h>
#include <atheris/cpu/init.h>

#include <atheris/sched/user.h>

#include <stdio.h>
#include <string.h>

namespace echis
{
    extern "C" void test_user_program();
    extern "C" void test_user_program_end();

    void SetupTestUserProc(sched::Process& proc)
    {
        proc.getAddressSpace().switchTo();

        pmm::physaddr page = pmm::GetPage();
        atheris::vm::MapPage(&proc.getAddressSpace(),
                             page,
                             0x69000,
                             atheris::vm::flags::present | atheris::vm::flags::write | atheris::vm::flags::user);

        char* start = reinterpret_cast<char*>(test_user_program);
        char* end   = reinterpret_cast<char*>(test_user_program_end);
        memcpy(reinterpret_cast<void*>(0x69000), start, end - start);

        pmm::physaddr stackPage = pmm::GetPage();
        atheris::vm::MapPage(&proc.getAddressSpace(),
                             stackPage,
                             0xFFFFFF0000000000,
                             atheris::vm::flags::present | atheris::vm::flags::write);
        proc.getMainThread()->getKernelStack().size = pmm::GetPageSize();
        proc.getMainThread()->getKernelStack().top  = 0xFFFFFF0000000000 + pmm::GetPageSize();

        proc.getMainThread()->getUserStack().size = pmm::GetPageSize();
        proc.getMainThread()->getUserStack().top  = reinterpret_cast<uint64_t>(vm::GetPages(&proc.getAddressSpace(),
                                                                 1,
                                                                 atheris::vm::flags::write | atheris::vm::flags::user)) + pmm::GetPageSize();
    }

    void kernel_main()
    {
        atheris::framebuffer::Init();
        atheris::console::Init();

        atheris::pm::Init();
        atheris::vm::Init();
        mm::Init();

        atheris::cpu::Init();

        atheris::cpu::smp::Init();
        atheris::timer::Init();
        
        fs::tmpfs::Init();
        fs::initrd::Init();

        fs::vfs::Node* node = fs::vfs::lookup("tmp:test.txt");
        char buf[100];
        size_t count = 100;
        node->read(buf, &count);
        console::PutString(buf, count, 0xffff00, 0);

        sched::Process proc(0x69000);
        sched::Process proc1(0x69000);
        sched::Process proc2(0x69000);
        SetupTestUserProc(proc);
        SetupTestUserProc(proc1);
        SetupTestUserProc(proc2);
        sched::AddProcess(&proc);
        sched::AddProcess(&proc1);
        sched::AddProcess(&proc2);

        sched::Start();

        atheris::cpu::Halt();
    }
}