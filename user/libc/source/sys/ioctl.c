#include <sys/ioctl.h>
#include <sys/syscall.h>

int ioctl(int fd, unsigned long op, void *argp)
{
    return syscall3(SYS_ioctl, fd, op, (uint64_t)argp);
}
