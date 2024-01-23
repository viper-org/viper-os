#ifndef VIPEROS_ECHIS_SYSCALL_PROC_EXIT_H
#define VIPEROS_ECHIS_SYSCALL_PROC_EXIT_H 1

#include <stddef.h>

namespace echis
{
    namespace syscall
    {
        void exit(int status);
    }
}

#endif // VIPEROS_ECHIS_SYSCALL_PROC_EXIT_H