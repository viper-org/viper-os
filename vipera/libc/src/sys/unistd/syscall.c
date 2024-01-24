#include <sys/unistd.h>

long syscall0(long number)
{
    long ret;
    asm volatile("syscall" : "=a"(ret) : "a"(number));
    return ret;
}

long syscall1(long number, long arg0)
{
    long ret;
    asm volatile("syscall" : "=a"(ret) : "a"(number), "D"(arg0));
    return ret;
}

long syscall2(long number, long arg0, long arg1)
{
    long ret;
    asm volatile("syscall" : "=a"(ret) : "a"(number), "D"(arg0), "S"(arg1));
    return ret;
}

long syscall3(long number, long arg0, long arg1, long arg3)
{
    long ret;
    asm volatile("syscall" : "=a"(ret) : "a"(number), "D"(arg0), "S"(arg1), "d"(arg3));
    return ret;
}   