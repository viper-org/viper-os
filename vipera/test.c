#include <sys/unistd.h>
#include <stdlib.h>
#include <signal.h>

unsigned long strlen(const char* data)
{
    unsigned long ret = 0;
    while(data[ret])
        ++ret;
    return ret;
}

int puts(const char* message)
{
    return write(1, message, strlen(message));
}

void sigusr1_handler(int a)
{
    puts("SIGUSR1 Received.\n");
}

void _start()
{
    signal(SIGUSR1, sigusr1_handler);
    raise(SIGUSR1);
    exit(42);
}