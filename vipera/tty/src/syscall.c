#include <syscall.h>

int open(const char* path, int flags)
{
    int ret;
    asm volatile("syscall" : "=a"(ret) : "a"(2), "D"(path), "S"(flags));
    return ret;
}

size_t read(int fd, void* buf, size_t count)
{
    size_t ret;
    asm volatile("syscall" : "=a"(ret) : "a"(0), "D"(fd), "S"(buf), "d"(count));
    return ret;
}

size_t write(int fd, const void* buf, size_t count)
{
    size_t ret;
    asm volatile("syscall" : "=a"(ret) : "a"(1), "D"(fd), "S"(buf), "d"(count));
    return ret;
}

int close(int fd)
{
    int ret;
    asm volatile("syscall" : "=a"(ret) : "a"(3), "D"(fd));
    return ret;
}

int ioctl(int fd, unsigned long request, void* arg)
{
    int ret;
    asm volatile("syscall" : "=a"(ret) : "a"(4), "D"(fd), "S"(request), "d"(arg));
    return ret;
}

void* mmap(size_t length)
{
    void* ret;
    asm volatile("syscall" : "=a"(ret) : "a"(5), "D"(length));
    return ret;
}

size_t seek(int fd, size_t offset)
{
    size_t ret;
    asm volatile("syscall" : "=a"(ret) : "a"(6), "D"(fd), "S"(offset));
    return ret;
}

int pipe(int fds[2])
{
    int ret;
    asm volatile("syscall" : "=a"(ret) : "a"(7), "D"(fds));
    return ret;
}

int spawn(const char* path)
{
    int ret;
    asm volatile("syscall" : "=a"(ret) : "a"(8), "D"(path));
    return ret;
}

int poll(int fd)
{
    int ret;
    asm volatile("syscall" : "=a"(ret) : "a"(9), "D"(fd));
    return ret;
}