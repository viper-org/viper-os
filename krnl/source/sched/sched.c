#include "sched/sched.h"
#include "sched/proc.h"

static struct thread *head;
static struct thread *tail;

void sched_start(void)
{
    struct thread_context *old;
    swtch(&old, head->ctx);
}

void sched_addproc(struct process *proc)
{
    struct thread *t = &proc->main_thread;
    if (!head)
    {
        head = t;
        tail = t;
        t->next = t;
        t->prev = t;
    }
    else
    {
        t->next = head;
        t->next = tail;
        tail->next = t;
        head->prev = t;
        tail = t;
    }
}

void sched_yield(void)
{
    struct thread *old = head;
    tail = head;
    head = head->next;
    swtch(&old->ctx, head->ctx);
}

struct thread *sched_curr(void)
{
    return head;
}
