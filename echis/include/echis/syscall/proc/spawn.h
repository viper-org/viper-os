#ifndef VIPEROS_ECHIS_SYSCALL_PROC_SPAWN_H
#define VIPEROS_ECHIS_SYSCALL_PROC_SPAWN_H 1

#include <stddef.h>

namespace echis
{
    namespace syscall
    {
        int spawn(const char* path);
    }
}

#endif // VIPEROS_ECHIS_SYSCALL_PROC_SPAWN_H