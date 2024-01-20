.text
.globl syscall_handler
.extern syscall_dispatcher
.type syscall_handler, @function
syscall_handler:
    movq %rsp, %gs:0x10 # Save user stack
    movq %gs:0x8, %rsp # Load kernel stack

    pushq %gs:0x10
    
    pushq %rcx
    pushq %r11

    pushq %rbp
    pushq %rbx
    pushq %rbp
    pushq %r12
    pushq %r13
    pushq %r14
    pushq %r15

    pushq %rax
    pushq %rdi
    pushq %rsi
    pushq %rdx
    pushq %r10
    pushq %r9
    pushq %r8

    movq %rsp, %rdi
    call syscall_dispatcher

    popq %r8
    popq %r9
    popq %r10
    popq %rdx
    popq %rsi
    popq %rdi
    popq %rax

    popq %r15
    popq %r14
    popq %r13
    popq %r12
    popq %rbp
    popq %rbx
    popq %rbp

    popq %r11
    popq %rcx

    popq %rsp # Restore saved user stack

    sysretq
