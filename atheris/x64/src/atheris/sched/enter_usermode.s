.text
.globl enter_usermode
enter_usermode:
    movq %rdi, %rcx

    cli

    movq %rsi, %rsp

    xorq %rbp, %rbp
    xorq %rax, %rax
    xorq %rbx, %rbx
    xorq %rdx, %rdx
    xorq %rsi, %rsi
    xorq %rdi, %rdi
    xorq %r8, %r8
    xorq %r9, %r9
    xorq %r10, %r10
    xorq %r12, %r12
    xorq %r13, %r13
    xorq %r14, %r14
    xorq %r15, %r15

    movq $0x202, %r11

    sysretq
