.text
.extern sched_yield
.globl start_idle_proc
.type start_idle_proc, @function
start_idle_proc:
    sti
1:
    call sched_yield
    jmp 1b