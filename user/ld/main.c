void _start(void)
{
    __asm__ volatile ("syscall" :: "a"(67), "D"("Hello, world!"));
    __asm__("1: pause; jmp 1b" ::: "memory");
}
