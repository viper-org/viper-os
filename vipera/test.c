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
    asm volatile("syscall" :: "a"(10), "D"(signum), "S"(handler));
    return ret;
}

void sigsegv_handler(int a)
{
    puts("SIGSEGV Received.\n");
    while(1) asm("pause");
}

void _start()
{
    sigaction(11, sigsegv_handler);
    //puts("Hello from program!\n");
    //puts("Testaaaaaaa\n");
    *(char*)(0x123456789) = 'a';
    while(1) asm("pause");
}