extern "C" void _start()
{
    asm volatile("1: cli; hlt; jmp 1b");
}