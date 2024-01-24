#include <stdlib.h>
#include <sys/unistd.h>

_Noreturn void exit(int exit_code)
{
    _exit(exit_code);
}