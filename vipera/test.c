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

void print(const char* message)
{
    asm volatile("syscall" :: "a"(69), "D"(message));
}

#define READ  1
#define WRITE 2

void _start(void)
{
    int fd = open("tmp:test.txt", READ | WRITE);

    char newMessage[] = "This was written from userspace";
    write(fd, newMessage, 33);

    char buf[33];
    read(fd, buf, 33);

    print(buf);

    
    while(1) asm("pause");
}
