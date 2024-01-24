#include <stdlib.h>
#include <sys/unistd.h>

_Noreturn void _Exit(int exit_code)
{
    _exit(exit_code);
}