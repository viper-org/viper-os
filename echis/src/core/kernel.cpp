#include <mm/pmm.h>
#include <mm/heap.h>

#include <sched/process.h>

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
        pmm::physaddr page = pmm::GetPage();
        atheris::vm::MapPage(&proc.getAddressSpace(),
                             page,
                             0x69000,
                             atheris::vm::flags::present | atheris::vm::flags::write | atheris::vm::flags::user);

        proc.getAddressSpace().switchTo();

        char* start = reinterpret_cast<char*>(test_user_program);
        char* end   = reinterpret_cast<char*>(test_user_program_end);
        memcpy(reinterpret_cast<void*>(0x69000), start, end - start);

        pmm::physaddr stackPage = pmm::GetPage();
        atheris::vm::MapPage(&proc.getAddressSpace(),
                             stackPage,
                             0xFFFFFFFFFFFE0000,
                             atheris::vm::flags::present | atheris::vm::flags::write);
        proc.getMainThread()->getKernelStack().size = pmm::GetPageSize();
        proc.getMainThread()->getKernelStack().top = reinterpret_cast<void*>(0xFFFFFFFFFFFE0000 + pmm::GetPageSize());
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
        /*atheris::timer::Subscribe([](){
            printf("Timer on CPU#%i ", static_cast<int>(atheris::cpu::core::id));
        });*/

        sched::Process proc(0x69000);
        SetupTestUserProc(proc);
        atheris::sched::UserInit(proc.getMainThread());

        atheris::cpu::Halt();
    }
}