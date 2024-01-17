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

void print(const char* message)
{
    asm volatile("syscall" :: "a"(69), "D"(message));
}

void _start(void)
{
    int fd = open("tmp:test.txt", 1);
    char buf[20];
    read(fd, buf, 20);
    print(buf);
    while(1) asm("pause");
}
