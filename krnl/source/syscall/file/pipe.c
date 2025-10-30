#include "syscall/syscalls.h"

#include "sched/pipe.h"

int sys_pipe(int fds[2])
{
    make_pipe(fds);
    return 0;
}
