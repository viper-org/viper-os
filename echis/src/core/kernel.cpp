#include <mm/pmm.h>
#include <mm/heap.h>
#include <mm/vmm.h>

#include <sched/process.h>
#include <sched/sched.h>

#include <fs/tmpfs.h>
#include <fs/devfs.h>
#include <fs/initrd.h>

#include <driver/framebuffer.h>

#include <ldr/elf.h>

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
        fs::vfs::Node* node = fs::vfs::lookup("tmp:test.exe");
        char* buffer = new char[0x4000];
        size_t count = 0x4000;
        node->read(buffer, &count);

        elf::Executable exec = elf::Load(buffer, &proc.getAddressSpace());
        proc.getMainThread()->setStartingAddress(exec.entryPoint);

        delete[] buffer;

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
        fs::devfs::Init();

        framebuffer::SetupDeviceFile();

        sched::Process proc(0);
        SetupTestUserProc(proc);
        sched::AddProcess(&proc);

        sched::Start();

        atheris::cpu::Halt();
    }
}