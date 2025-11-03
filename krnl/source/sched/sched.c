#include "sched/sched.h"
#include "sched/proc.h"

#include "cpu/tss.h"

static struct thread *head;
static struct thread *tail;

void start_idle_proc(void);

void sched_start(void)
{
    head->timeslice = 3;
    struct thread_context *old;
    tss_set_rsp0(head->krnl_stack.top);
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
        t->prev = tail;
        tail->next = t;
        head->prev = t;
        tail = t;
    }
}

void sched_yield(void)
{
    if (!head) return;

    struct thread *old = head;
    tail = head;
    head = head->next;

    if (old == head) return;

    ctx_switch(old, head);
}

void sched_tick(void)
{
    if (head)
    {
        if (!(--head->timeslice))
        {
            head->next->timeslice = 3;
            sched_yield();
        }
    }
}

void sched_blockcurr(void)
{
    struct thread *old = head;
    tail->next = head->next;
    head->next->prev = tail;
    head = head->next;

    if (head == old)
    {
        head = NULL;
        tail = NULL;
        start_idle_proc();
    }

    ctx_switch(old, head);
}

void sched_blockone(struct thread *t)
{
    if (t == head)
    {
        sched_blockcurr();
        return;
    }

    t->prev->next = t->next;
    t->next->prev = t->prev;
}

void sched_readdthread(struct thread *t)
{
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
        t->prev = tail;
        tail->next = t;
        head->prev = t;
        tail = t;
    }
}

struct thread *sched_curr(void)
{
    return head;
}
