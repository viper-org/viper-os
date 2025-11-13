.text
.globl enter_signal_handler
.type enter_signal_handler, @function
enter_signal_handler:
    movq %rcx, %rax
    movq %rsi, %rcx
    movq %rdx, %rsp

    pushq 0x88(%rdi) # r15
    pushq 0x80(%rdi) # r14
    pushq 0x78(%rdi) # r13
    pushq 0x70(%rdi) # r12
    pushq 0x68(%rdi) # r11
    pushq 0x60(%rdi) # r10
    pushq 0x58(%rdi) # r9
    pushq 0x50(%rdi) # r8
    pushq 0x48(%rdi) # rdi
    pushq 0x40(%rdi) # rsi
    pushq 0x38(%rdi) # rdx
    pushq 0x30(%rdi) # rcx
    pushq 0x28(%rdi) # rbx
    pushq 0x20(%rdi) # rax

    pushq 0x18(%rdi) # rflags
    pushq 0x10(%rdi) # rbp
    pushq 0x08(%rdi) # rsp
    pushq 0x00(%rdi) # rip

    movq $0x202, %r11

    pushq %rax # Return code (0x516 is returnable, 0x519 is not)

    sysretq

.globl return_from_signal_handler
.type return_from_signal_handler, @function
return_from_signal_handler:
    movq %rdi, %rsp

    popq %rcx
    popq %gs:0x10 # original stack restored later
    popq %rbp
    popq %r11

    popq %rax
    popq %rbx
    addq $0x8, %rsp # rcx
    popq %rdx
    popq %rsi
    popq %rdi
    popq %r8
    popq %r9
    popq %r10
    addq $0x8, %rsp # r11
    popq %r12
    popq %r13
    popq %r14
    popq %r15

    movq %gs:0x10, %rsp

    sysretq