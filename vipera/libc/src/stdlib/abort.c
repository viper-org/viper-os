#include <stdlib.h>
#include <sys/unistd.h>

_Noreturn void abort()
{
    _exit(EXIT_FAILURE);
}