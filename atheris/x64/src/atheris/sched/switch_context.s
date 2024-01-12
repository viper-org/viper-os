.text
.globl switch_context
.type switch_context, @function
switch_context:
    movq %rdi, %rax
    movq %rsi, %rdx

    pushq %r15
    pushq %r14
    pushq %r13
    pushq %r12
    pushq %rsi
    pushq %rdi
    pushq %rbx
    pushq %rbp

    movq %rsp, (%rax)
    movq %rdx, %rsp

    popq %rbp
    popq %rbx
    popq %rdi
    popq %rsi
    popq %r12
    popq %r13
    popq %r14
    popq %r15
    ret