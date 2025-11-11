#include "syscall/syscalls.h"

#include "sched/proc.h"
#include "sched/sched.h"
#include <string.h>

char *sys_getcwd(char buf[], size_t size)
{
    struct process *proc = sched_curr()->owner;

    size_t len = strlen(proc->cwd) + 1;
    if (len > size) return NULL;

    memcpy(buf, proc->cwd, len);
    return buf;
}
