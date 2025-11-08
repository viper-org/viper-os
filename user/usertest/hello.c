#include <unistd.h>

void _start(void)
{
    write(1, "Hello, world!\n", 15);
    _exit(0);
}
