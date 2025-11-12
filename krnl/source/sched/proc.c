#include "sched/proc.h"
#include "sched/sched.h"
#include "sched/pipe.h"

#include "mm/kheap.h"
#include "mm/vm.h"

#include "cpu/tss.h"
#include "cpu/cpu.h"

#include "event/object.h"

#include "thread/proc.h"

#include <string.h>

static int npid = 1;

extern void prepare_thread(uint64_t entry, struct thread_context **ctx, uint64_t stack, struct thread *t, uint64_t pml4);
extern void enter_usermode(uint64_t rip, uint64_t rsp);
void usermode_setup(struct thread *t);

struct process *proc_table = NULL;
struct process *done_proc_table = NULL;

void init_proc(struct process *proc, const char *wd)
{
    proc->pid = npid++;
    proc->has_addrspace = 1;
    proc->addr_space = make_addrspace();
    proc->cwd = strdup(wd);
    memset(proc->fds, 0, sizeof proc->fds);

    struct thread *t = &proc->main_thread;
    t->owner = proc;
    void *kstack_base = vm_getpages(NULL, 8, 0, NULL);
    t->krnl_stack = (struct stack) {
        .top = (uint64_t)kstack_base + 8 * 0x1000,
        8 * 0x1000
    };
    void *ustack_base = vm_getpages(&proc->addr_space, 8, 0, NULL);
    t->usr_stack = (struct stack) {
        .top = (uint64_t)ustack_base + 8 * 0x1000,
        8 * 0x1000
    };
    t->exit_event = NULL;

    prepare_thread((uint64_t)usermode_setup, &t->ctx, t->krnl_stack.top, t, t->owner->addr_space.pml4);

    proc->next = proc_table;
    proc->prev = NULL;
    if (proc_table)
        proc_table->prev = proc;
    proc_table = proc;
}

void init_kproc(struct process *proc, uint64_t entry)
{
    proc->pid = npid++;
    proc->has_addrspace = 0;
    proc->cwd = "/";
    memset(proc->fds, 0, sizeof proc->fds);

    struct thread *t = &proc->main_thread;
    t->owner = proc;
    void *kstack_base = vm_getpages(NULL, 8, 0, NULL);
    t->krnl_stack = (struct stack) {
        .top = (uint64_t)kstack_base + 8 * 0x1000,
        8 * 0x1000
    };
    t->exit_event = NULL;

    prepare_thread(entry, &t->ctx, t->krnl_stack.top, t, vm_get_kaddrspace()->pml4);

    proc->next = proc_table;
    proc->prev = NULL;
    if (proc_table)
        proc_table->prev = proc;
    proc_table = proc;
}

struct process *alloc_proc(int ppid, const char *wd)
{
    struct process *ret = kheap_alloc(sizeof(struct process));
    ret->ppid = ppid;
    init_proc(ret, wd);
    return ret;
}

int proc_addfd(struct process *proc, struct vnode *node, enum openmode mode)
{
    for (int i = 0; i < NFD; ++i)
    {
        if (!proc->fds[i].vnode && !proc->fds[i].pipe)
        {
            proc->fds[i].vnode = node;
            proc->fds[i].flags = mode;
            return i;
        }
    }
    return -1;
}

int proc_add_pipefd(struct process *proc, struct pipe *pipe)
{
    for (int i = 0; i < NFD; ++i)
    {
        if (!proc->fds[i].vnode && !proc->fds[i].pipe)
        {
            proc->fds[i].pipe = pipe;
            return i;
        }
    }
    return -1;
}

int proc_get_freefd(struct process *proc)
{
    for (int i = 0; i < NFD; ++i)
    {
        if (!proc->fds[i].vnode && !proc->fds[i].pipe)
        {
            return i;
        }
    }
    return -1;
}

struct process *find_proc(int pid)
{
    struct process *curr = proc_table;
    while (curr)
    {
        if (curr->pid == pid) return curr;
    }
    return NULL;
}

struct process **proc_get_done(void)
{
    return &done_proc_table;
}


void thread_kill(struct thread *t, int code)
{
    if (t->exit_event)
    {
        ready_event(&t->exit_event->obj);
        t->exit_event->exit_code = code;
    }
    struct process *proc = t->owner;
    struct process *next = proc->next;
    struct process *prev = proc->prev;

    if (prev)
        prev->next = next;
    next->prev = prev;
    
    proc->next = done_proc_table;
    proc->prev = NULL;
    if (done_proc_table)
        done_proc_table->prev = proc;
    done_proc_table = proc;

    kthread_proc_awake();

    sched_blockone(t);
}

void ctx_switch(struct thread *old, struct thread *new)
{
    if (!new || !new->owner)
    {
        int a = 1;
    }
    tss_set_rsp0(new->krnl_stack.top);
    get_core()->kstack = new->krnl_stack.top;
    old->ustack_save = get_core()->ustack;
    get_core()->ustack = new->ustack_save;
    if (new->owner->has_addrspace)
        vm_set_curr(&new->owner->addr_space);
    else
        vm_set_curr(vm_get_kaddrspace());
    swtch(&old->ctx, new->ctx);
}

void usermode_setup(struct thread *t)
{
    vm_switch_to(&t->owner->addr_space);
    get_core()->kstack = t->krnl_stack.top;
    enter_usermode(t->entry, t->ustack_save);
}
