#include "thread/proc.h"

#include "sched/sched.h"

#include "mm/kheap.h"

static struct process proc;

void kthread_proc_cleaner(void)
{
    while (1)
    {
        struct process **done = proc_get_done();

        struct process *curr = *done;
        while (curr)
        {
            struct process *next = curr->next;
            *done = next; // in case we get interrupted before freeing curr

            if (curr->has_addrspace)
            {
                free_addrspace(&curr->addr_space);
            }

            kheap_free(curr);
            curr = next;
        }

        sched_blockcurr(); // nothing to do
    }
}

void kthread_proc_init(void)
{
    init_kproc(&proc, (uint64_t)kthread_proc_cleaner);
    sched_addproc(&proc);
    sched_blockone(&proc.main_thread);
}

void kthread_proc_awake(void)
{
    sched_readdthread(&proc.main_thread);
}
