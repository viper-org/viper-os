#ifndef VIPEROS_SCHED_PROC
#define VIPEROS_SCHED_PROC 1

#include "sched/procfd.h"

#include "cpu/swtch.h"

#include "mm/vm.h"

#include <stdint.h>

struct stack
{
    uint64_t top;
    size_t size;
};

struct process;
struct pipe;
struct exit_event_object;

struct thread
{
    struct process *owner;
    struct stack usr_stack;
    struct stack krnl_stack;

    struct thread_context *ctx;
    uint64_t entry;

    struct exit_event_object *exit_event;

    struct thread *prev;
    struct thread *next;
    struct thread *wnext;
};

#define NFD 16 // will need more later probably

struct process
{
    int pid;
    struct thread main_thread;
    struct addrspace addr_space;

    struct proc_fd fds[NFD];

    struct process *next; // linked list
};

struct process *alloc_proc(uint64_t entry);

int proc_addfd(struct process *proc, struct vnode *node, enum openmode mode);
int proc_add_pipefd(struct process *proc, struct pipe *pipe);
int proc_get_freefd(struct process *proc);
struct process *find_proc(int pid);

void thread_kill(struct thread *t, int code);

void ctx_switch(struct thread *old, struct thread *new);

#endif // VIPEROS_SCHED_PROC
