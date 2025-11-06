#include <poll.h>
#include <sys/syscall.h>

int poll1(int fd)
{
    return syscall1(SYS_poll1, fd);
}

int poll(int *fds, size_t nfds)
{
    return syscall2(SYS_poll, (uint64_t)fds, nfds);
}
