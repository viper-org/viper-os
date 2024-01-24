#include <sys/unistd.h>
#include <sys/syscall.h>

int close(int fd)
{
    return syscall1(SYS_close, fd);
}