#include "sched/proc.h"
#include "sched/sched.h"

#include "mm/kheap.h"
#include "mm/vm.h"

#include "cpu/tss.h"

#include "event/object.h"

static int npid = 0;

extern void prepare_thread(uint64_t entry, struct thread_context **ctx, uint64_t stack, struct thread *t, uint64_t pml4);

struct process *alloc_proc(uint64_t entry)
{
    struct process *ret = kheap_alloc(sizeof(struct process));
    ret->pid = npid++;
    ret->addr_space = make_addrspace();

    struct thread *t = &ret->main_thread;
    t->entry = entry;
    t->owner = ret;
    void *kstack_base = vm_getpages(NULL, 8);
    t->krnl_stack = (struct stack) {
        .top = (uint64_t)kstack_base + 8 * 0x1000,
        8 * 0x1000
    };
    t->exit_event = NULL;

    prepare_thread(entry, &t->ctx, t->krnl_stack.top, t, t->owner->addr_space.pml4);
    
    return ret;
}

void thread_kill(struct thread *t)
{
    if (t->exit_event)
    {
        ready_event(&t->exit_event->obj);
    }
    sched_blockone(t);
}

void ctx_switch(struct thread *old, struct thread *new)
{
    tss_set_rsp0(new->krnl_stack.top);
    swtch(&old->ctx, new->ctx);
}
