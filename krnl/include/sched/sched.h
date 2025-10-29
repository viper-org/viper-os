#ifndef VIPEROS_SCHED_SCHED
#define VIPEROS_SCHED_SCHED 1

#include "sched/proc.h"

void sched_start(void);
void sched_addproc(struct process *proc);

void sched_yield(void);

void sched_blockcurr(void);
// this should only be used when an existing thread is unblocked
void sched_readdthread(struct thread *t);

struct thread *sched_curr(void);

#endif // VIPEROS_SCHED_SCHED
