#include <sys/unistd.h>
#include <sys/syscall.h>

ssize_t write(int fd, const void *buf, size_t count)
{
    return syscall3(SYS_write, fd, (long)buf, count);
}