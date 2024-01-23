unsigned long strlen(const char* data)
{
    unsigned long ret = 0;
    while(data[ret])
        ++ret;
    return ret;
}

void puts(const char* message)
{
    asm volatile("syscall" :: "a"(1), "D"(3), "S"(message), "d"(strlen(message)));
}

int spawn(const char* path)
{
    asm volatile("syscall" :: "a"(8), "D"(path));
    return 0;
}

int sigaction(int signum, void(*handler)(int))
{
    int ret;
    asm volatile("syscall" : "=a"(ret) : "a"(10), "D"(signum), "S"(handler));
    return ret;
}

int raise(int sig)
{
    int ret;
    asm volatile("syscall" : "=a"(ret) : "a"(11), "D"(sig));
    return ret;
}

void sigusr1_handler(int a)
{
    puts("SIGUSR1 Received.\n");
}

void _start()
{
    sigaction(10, sigusr1_handler);
    raise(10);
    while(1) asm("pause");
}