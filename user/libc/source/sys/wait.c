#include "sys/wait.h"
#include "sys/syscall.h"

pid_t waitpid(pid_t pid, int *status, int options)
{
    return syscall3(SYS_waitpid, pid, (uint64_t)status, options);
}
