#ifndef VIPEROS_SCHED_SCHED
#define VIPEROS_SCHED_SCHED 1

#include "sched/proc.h"

void sched_start(void);
void sched_addproc(struct process *proc);

void sched_yield(void);

struct thread *sched_curr(void);

#endif // VIPEROS_SCHED_SCHED
