.text
.globl prepare_thread
.type prepare_thread, @function
prepare_thread:
    movq %rsp, %r11
    movq %rdx, %rsp

    pushq %rdi
    pushq $0x0
    pushq $0x0
    pushq $0x0
    pushq $0x0
    pushq $0x0
    pushq %rcx
    pushq $0x0
    pushq $0x0

    movq %rsp, (%rsi)
    movq %r11, %rsp

    ret