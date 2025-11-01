#include <sched.h>
#include <sys/syscall.h>

void sched_yield(void)
{
    syscall0(SYS_yield);
}
