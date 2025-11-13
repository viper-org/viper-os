#include "signal/signal.h"

#include "mm/kheap.h"
#include "mm/vm.h"

#include "sched/sched.h"

void enter_signal_handler(struct saved_user_ctx *ctx, void (*handler)(int), uint64_t stack, int return_to);
void return_from_signal_handler(uint64_t rsp);

int signal_return_to(enum signal_type type)
{
    switch (type)
    {
        case SIGKILL:
            return 0x519;
        default:
            return 0x516;
    }
}

void signal_handler(struct saved_user_ctx *ctx, void (*handler)(int), int return_to)
{
    struct process *proc = sched_curr()->owner;
    if (proc->has_addrspace)
        vm_switch_to(&proc->addr_space);
    else
        vm_switch_to(vm_get_kaddrspace());

    enter_signal_handler(ctx, handler, sched_curr()->signal_stack.top, return_to);
}

void signal_return(uint64_t rsp)
{
    return_from_signal_handler(rsp);
}


void deliver_to_current(enum signal_type sig)
{
    struct thread *current = sched_curr();
    struct pending_signal *curr = current->signals;
    
    while (curr)
    {
        if (curr->type == SIGNONE)
        {
            curr->type = sig;
            return;
        }
        curr = curr->next;
    }
    struct pending_signal *new = kheap_alloc(sizeof (struct pending_signal));
    new->type = sig;
    new->next = current->signals;
    current->signals = new;
}

struct pending_signal *check_incoming(void)
{
    struct thread *current = sched_curr();
    if (!current) return NULL;

    struct pending_signal *curr = current->signals;
    while (curr)
    {
        if (curr->type != SIGNONE)
        {
            return curr;
        }

        curr = curr->next;
    }

    return NULL;
}

void (*sig_gethandler(struct pending_signal *sig))(int)
{
    return sched_curr()->signal_handlers[sig->type];
}
