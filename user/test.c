int open(const char *path, unsigned short mode)
{
    int ret;
    __asm__ volatile("syscall" : "=a"(ret) : "a"(2), "D"(path), "S"(mode));
    return ret;
}

void _start(void)
{
    int fd = open("/dev/fb", 3);
    while (1) __asm__ volatile("pause");
}
