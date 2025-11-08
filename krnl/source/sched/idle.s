.text
.extern sched_yield
.globl start_idle_proc
.type start_idle_proc, @function
start_idle_proc:
1:
    sti
    jmp 1b