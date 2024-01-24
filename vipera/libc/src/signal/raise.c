#include <stdlib.h>
#include <sys/syscall.h>

int raise(int sig)
{
    return syscall1(SYS_raise, sig);
}