#include <mm/heap.h>

#include <atheris/driver/framebuffer.h>
#include <atheris/driver/console.h>
#include <atheris/driver/timer.h>

#include <atheris/mm/pm.h>
#include <atheris/mm/vm.h>

#include <atheris/cpu/smp.h>
#include <atheris/cpu/core.h>
#include <atheris/cpu/halt.h>
#include <atheris/cpu/init.h>

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

        atheris::cpu::Init();

        atheris::cpu::smp::Init();
        atheris::timer::Init();
        atheris::timer::Subscribe([](){
            printf("Timer on CPU#%i ", static_cast<int>(atheris::cpu::core::id));
        });

        atheris::cpu::Halt();
    }
}