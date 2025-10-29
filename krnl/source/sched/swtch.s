
.text
.globl swtch
.type swtch, @function
swtch:
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

    movq %cr3, %rcx
    pushq %rcx

    movq %rsp, (%rax)
    movq %rdx, %rsp
    
    popq %rcx
    movq %rcx, %cr3

    popq %rbp
    popq %rbx
    popq %rdi
    popq %rsi
    popq %r12
    popq %r13
    popq %r14
    popq %r15
    ret
