#include <stdlib.h>
#include <sys/unistd.h>

_Noreturn void quick_exit(int exit_code)
{
    _exit(exit_code);
}