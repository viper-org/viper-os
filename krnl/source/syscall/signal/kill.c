#include "syscall/syscalls.h"

#include "sched/proc.h"

#include "signal/signal.h"

int sys_kill(int pid, int sig)
{
    if (sig < 0 || sig > 32) return 1;

    struct process *proc = find_proc(pid);
    if (!proc) return 1;

    deliver_to_one(&proc->main_thread, sig);

    return 0;
}
