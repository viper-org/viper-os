#include "sched/proc.h"

#include "mm/kheap.h"
#include "mm/vm.h"

static int npid = 0;

extern void prepare_thread(uint64_t entry, struct thread_context **ctx, uint64_t stack, struct thread *t);

struct process *alloc_proc(uint64_t entry)
{
    struct process *ret = kheap_alloc(sizeof(struct process));
    ret->pid = npid++;

    struct thread *t = &ret->main_thread;
    t->entry = entry;
    t->owner = ret;
    void *kstack_base = vm_getpages(NULL, 8);
    t->krnl_stack = (struct stack) {
        .top = (uint64_t)kstack_base + 8 * 0x1000,
        8 * 0x1000
    };

    prepare_thread(entry, &t->ctx, t->krnl_stack.top, t);
    
    return ret;
}
