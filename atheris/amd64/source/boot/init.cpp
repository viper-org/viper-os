#include <echis/main.h>

#include <atheris/driver/debugcon.h>

#include <mm/pm.h>

extern "C" void _start()
{
    atheris::driver::debugcon::WriteChar('\n');

    atheris::amd64::pm::Initialize();

    echis::KernelMain();

    for(;;) asm("pause");
}