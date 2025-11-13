#include <signal.h>
#include <stdlib.h>
#include <stdio.h>

// example program to demonstrate signals

void handler(int n)
{
    (void)n;
    puts("SIGSEGV");
}

int main(void)
{
    sigaction(SIGSEGV, handler);
    *(int*)12 = 0;
}
