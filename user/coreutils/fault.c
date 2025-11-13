#include "sys/wait.h"
#include "unistd.h"
#include "stdlib.h"
#include <signal.h>
#include <stdio.h>

// example program to demonstrate signals

void handler(int n)
{
    (void)n;
    puts("SIGUSR1");
    exit(0);
}

int main(void)
{
    sigaction(SIGUSR1, handler);
    struct spawn x = {-1, -1, -1};
    char *argv[] = {"/bin/fault2"};
    int _;
    waitpid(spawn(argv[0], 1, argv, &x), &_, 0);
}
