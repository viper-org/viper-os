int open(const char *path, unsigned short mode)
{
    int ret;
    __asm__ volatile("syscall" : "=a"(ret) : "a"(2), "D"(path), "S"(mode) : "r11", "rcx");
    return ret;
}

long read(int fd, void *buf, unsigned long size)
{
    long ret;
    __asm__ volatile("syscall" : "=a"(ret) : "a"(0), "D"(fd), "S"(buf), "d"(size) : "r11", "rcx");
    return ret;
}

long write(int fd, const void *buf, unsigned long size)
{
    long ret;
    __asm__ volatile("syscall" : "=a"(ret) : "a"(1), "D"(fd), "S"(buf), "d"(size) : "r11", "rcx");
    return ret;
}

long lseek(int fd, long off, int whence)
{
    long ret;
    __asm__ volatile("syscall" : "=a"(ret) : "a"(8), "D"(fd), "S"(off), "d"(whence) : "r11", "rcx");
    return ret;
}

void dbg_print(const char *s)
{
    __asm__ volatile("syscall" :: "a"(67), "D"(s) : "r11", "rcx");
}

void _start(void)
{
    int fd = open("/tmp/usertest", 3);
    char buf[12];
    write(fd, "Hello", 6);
    lseek(fd, 0, 0); // SEEK_SET
    read(fd, buf, 11);
    dbg_print(buf);
    while (1) __asm__ volatile("pause");
}
