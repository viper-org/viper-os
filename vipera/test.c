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

void _start()
{
    puts("Hello from program!\n");
    puts("Testaaaaaaa\n");
    while(1) asm("pause");
}