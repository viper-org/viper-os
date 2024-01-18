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

        sched::Process* proc = sched::Process::Create("tmp:tty");
        sched::AddProcess(proc);

        sched::Start();

        atheris::cpu::Halt();
    }
}