#include <echis/main.h>

#include <atheris/driver/debugcon.h>

extern "C" void _start()
{
    atheris::driver::debugcon::WriteChar('\n');
    atheris::driver::debugcon::WriteChar('h');
    atheris::driver::debugcon::WriteChar('e');
    atheris::driver::debugcon::WriteChar('l');
    atheris::driver::debugcon::WriteChar('l');
    atheris::driver::debugcon::WriteChar('o');

    for(;;) asm("pause");
}