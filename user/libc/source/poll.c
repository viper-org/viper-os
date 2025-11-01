#include <poll.h>
#include <sys/syscall.h>

int poll1(int fd)
{
    return syscall1(SYS_poll1, fd);
}
