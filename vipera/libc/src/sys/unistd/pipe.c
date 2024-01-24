#include <sys/unistd.h>
#include <sys/syscall.h>

int pipe(int fds[2])
{
    return syscall1(SYS_pipe, (long)fds);
}