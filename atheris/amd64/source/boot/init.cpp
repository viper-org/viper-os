#include <echis/main.h>

#include <atheris/driver/debugcon.h>

extern "C" void _start()
{
    atheris::driver::debugcon::WriteChar('\n');

    echis::KernelMain();

    for(;;) asm("pause");
}