typedef unsigned long size_t;

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

void write(int fd, const void* buf, size_t count) // technically returns size_t but who cares
{
    asm volatile("syscall" :: "a"(1), "D"(fd), "S"(buf), "d"(count));
}

int close(int fd)
{
    int ret;
    asm volatile("syscall" : "=a"(ret) : "a"(3), "D"(fd));
    return ret;
}

int ioctl(int fd, unsigned long request, char* arg)
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

void print(const char* message)
{
    asm volatile("syscall" :: "a"(69), "D"(message));
}

void memset(void* s, int c, size_t n)
{
    char* p = s;
    for (size_t i = 0; i < n; ++i)
    {
        p[i] = c;
    }
}

#define READ  1
#define WRITE 2

void _start(void)
{
    int fd = open("dev:fb", WRITE);
    unsigned long vert;
    ioctl(fd, 1, &vert);
    unsigned long pitch;
    ioctl(fd, 2, &pitch);

    void* buffer = mmap(vert * pitch);
    memset(buffer, 0, vert * pitch);
    write(fd, buffer, vert * pitch);
    
    while(1) asm("pause");
}
