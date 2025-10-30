struct stat
{
    long size;
};

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

int close(int fd)
{
    int ret;
    __asm__ volatile("syscall" : "=a"(ret) : "a"(3), "D"(fd) : "r11", "rcx");
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

int fstat(int fd, struct stat *statbuf)
{
    int ret;
    __asm__ volatile("syscall" : "=a"(ret) : "a"(5), "D"(fd), "S"(statbuf) : "r11", "rcx");
    return ret;
}

int stat(const char *path, struct stat *statbuf)
{
    int ret;
    __asm__ volatile("syscall" : "=a"(ret) : "a"(4), "D"(path), "S"(statbuf) : "r11", "rcx");
    return ret;
}

int getpid()
{
    int ret;
    __asm__ volatile("syscall" : "=a"(ret) : "a"(13) : "rcx", "r11");
    return ret;
}

int waitpid(int pid, int *status, int options)
{
    int ret;
    __asm__ volatile("syscall" : "=a"(ret) : "a"(12), "D"(pid), "S"(status), "d"(options) : "r11", "rcx");
    return ret;
}

void yield(void)
{
    __asm__ volatile("syscall" :: "a"(24) : "r11", "rcx");
}

void exit(int code)
{
    __asm__ volatile("syscall" :: "a"(60), "D"(code) : "r11", "rcx");
}

int pipe(int fds[2])
{
    int ret;
    __asm__ volatile("syscall" : "=a"(ret) : "a"(22), "D"(fds) : "rcx", "r11");
    return ret;
}

int poll1(int fd)
{
    int ret;
    __asm__ volatile("syscall" : "=a"(ret) : "a"(7), "D"(fd) : "rcx", "r11");
    return ret;
}

void *mmap(unsigned long len)
{
    void *ret;
    __asm__ volatile("syscall" : "=a"(ret) : "a"(9), "S"(len) : "rcx", "r11");
    return ret;
}

int spawn(const char *path)
{
    int ret;
    __asm__ volatile("syscall" : "=a"(ret) : "a"(25), "D"(path) : "rcx", "r11");
    return ret;
}

int dup(int fd)
{
    int ret;
    __asm__ volatile("syscall" : "=a"(ret) : "a"(33), "D"(fd) : "rcx", "r11");
    return ret;
}

int dup2(int old, int new)
{
    int ret;
    __asm__ volatile("syscall" : "=a"(ret) : "a"(34), "D"(old), "S"(new) : "rcx", "r11");
    return ret;
}

void _start(void)
{
    if (getpid() == 0)
    {
        int fds[2];
        pipe(fds);
        dup2(fds[1], 9);
        close(fds[1]);

        spawn("/tmp/usertest");
        poll1(fds[0]);
    
        char buf[7];
        read(fds[0], buf, 7);
        dbg_print(buf);
    }
    else // child
    {
        write(9, "Hello\n", 7);
        exit(0);
    }

    while (1) __asm__ volatile("pause");
}
