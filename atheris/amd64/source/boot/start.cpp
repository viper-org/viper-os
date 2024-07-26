#include <atheris/driver/debugcon.h>

extern "C" void _start()
{
    atheris::driver::debugcon::WriteChar('\n');
    atheris::driver::debugcon::Write("Hello World");
    for(;;) asm("pause");
}