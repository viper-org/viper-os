#include <mm/heap.h>

#include <atheris/driver/framebuffer.h>
#include <atheris/driver/console.h>

#include <atheris/mm/pm.h>
#include <atheris/mm/vm.h>

#include <atheris/cpu/smp.h>

#include <atheris/common/halt.h>

#include <stdio.h>

namespace echis
{
    void kernel_main()
    {
        atheris::framebuffer::Init();
        atheris::console::Init();

        atheris::pm::Init();
        atheris::vm::Init();
        mm::Init();

        atheris::cpu::smp::Init();

        atheris::Halt();
    }
}