#include <echis/main.h>

extern "C" void _start()
{
    for(;;) asm("pause");
}