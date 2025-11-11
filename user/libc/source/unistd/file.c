#include <unistd.h>
#include <sys/syscall.h>

ssize_t read(int fd, void *buf, size_t count)
{
    return syscall3(SYS_read, fd, (uint64_t)buf, count);
}

ssize_t write(int fd, const void *buf, size_t count)
{
    return syscall3(SYS_write, fd, (uint64_t)buf, count);
}

int open(const char *path, int flags)
{
    return syscall2(SYS_open, (uint64_t)path, flags);
}

int close(int fd)
{
    return syscall1(SYS_close, fd);
}

off_t lseek(int fd, off_t offset, int whence)
{
    return syscall3(SYS_lseek, fd, offset, whence);
}

int pipe(int pipefd[2])
{
    return syscall1(SYS_pipe, (uint64_t)pipefd);
}

int dup(int fd)
{
    return syscall1(SYS_dup, fd);
}

int dup2(int oldfd, int newfd)
{
    return syscall2(SYS_dup2, oldfd, newfd);
}

int mkdir(const char *path)
{
    return syscall1(SYS_mkdir, (uint64_t)path);
}
