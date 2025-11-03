#include <sys/syscall.h>

void _start(void)
{
    syscall1(67, (uint64_t)"in proc2");
    while (1) __asm__("pause");
}
