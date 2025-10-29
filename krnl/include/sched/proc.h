#ifndef VIPEROS_SCHED_PROC
#define VIPEROS_SCHED_PROC 1

#include "cpu/swtch.h"

#include "mm/vm.h"

#include <stdint.h>

struct stack
{
    uint64_t top;
    size_t size;
};

struct process;

struct thread
{
    struct process *owner;
    struct stack usr_stack;
    struct stack krnl_stack;

    struct thread_context *ctx;
    uint64_t entry;

    struct thread *prev;
    struct thread *next;
};

struct process
{
    int pid;
    struct thread main_thread;
    struct addrspace addr_space;
};

struct process *alloc_proc(uint64_t entry);

#endif // VIPEROS_SCHED_PROC
