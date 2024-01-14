void _start(void)
{
    const char* message = "Hello from C!";
    asm volatile("syscall" :: "D"(message));
    while(1) asm("pause");
}
 