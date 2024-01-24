#include <stdlib.h>
#include <sys/syscall.h>

void (*signal(int sig, void (*handler) (int))) (int)
{
    return (void (*)(int))syscall2(SYS_signal, sig, (long)handler);
}