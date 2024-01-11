.text
.globl syscall_handler
.extern syscall_dispatcher
.type syscall_handler, @function
syscall_handler:
    movq %rsp, %gs:0x10 # Save user stack
    movq %gs:0x8, %rsp # Load kernel stack
    
    pushq %rcx
    pushq %r11

    pushq %rbx
    pushq %rbp
    pushq %r12
    pushq %r13
    pushq %r14
    pushq %r15

    call syscall_dispatcher

    popq %r15
    popq %r14
    popq %r13
    popq %r12
    popq %rbp
    popq %rbx

    popq %r11
    popq %rcx

    movq %gs:0x10, %rsp # Restore saved user stack
    sysretq