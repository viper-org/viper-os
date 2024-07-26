.text
.globl exception_stub_table
.extern CommonExceptionHandler

.macro exception_err_stub vector
exception_stub_\vector:
    pushq $\vector
    jmp exception_frame_assembler
.endm

.macro exception_no_err_stub vector
exception_stub_\vector:
    pushq $0 # Dummy error code
    pushq $\vector
    jmp exception_frame_assembler
.endm

exception_frame_assembler:
    pushq %rbp
    movq %rsp, %rbp

    pushq %rax
    pushq %rbx
    pushq %rcx
    pushq %rdx
    pushq %rsi
    pushq %rdi
    pushq %r15
    pushq %r14
    pushq %r13
    pushq %r12
    pushq %r11
    pushq %r10
    pushq %r9
    pushq %r8

    movq %cr0, %rax
    pushq %rax
    movq %cr2, %rax
    pushq %rax
    movq %cr3, %rax
    pushq %rax
    movq %cr4, %rax
    pushq %rax

    movw $0x10, %ax
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %ss

    movq %rsp, %rdi # Pass the stack frame as the first parameter
    call CommonExceptionHandler

    popq %rax
    movq %rax, %cr4
    popq %rax
    movq %rax, %cr3
    popq %rax
    movq %rax, %cr2
    popq %rax
    movq %rax, %cr0

    popq %r8
    popq %r9
    popq %r10
    popq %r11
    popq %r12
    popq %r13
    popq %r14
    popq %r15
    popq %rdi
    popq %rsi
    popq %rdx
    popq %rcx
    popq %rbx
    popq %rax

    popq %rbp
    
    addq $0x10, %rsp

    iretq

exception_no_err_stub    0
exception_no_err_stub  1
exception_no_err_stub  2
exception_no_err_stub  3
exception_no_err_stub  4
exception_no_err_stub  5
exception_no_err_stub  6
exception_no_err_stub  7
exception_err_stub    8
exception_no_err_stub  9
exception_err_stub    10
exception_err_stub    11
exception_err_stub    12
exception_err_stub    13
exception_err_stub    14
exception_no_err_stub  15
exception_no_err_stub  16
exception_err_stub    17
exception_no_err_stub  18
exception_no_err_stub  19
exception_no_err_stub  20
exception_no_err_stub  21
exception_no_err_stub  22
exception_no_err_stub  23
exception_no_err_stub  24
exception_no_err_stub  25
exception_no_err_stub  26
exception_no_err_stub  27
exception_no_err_stub  28
exception_no_err_stub  29
exception_err_stub    30
exception_no_err_stub  31

.section .data
.macro exception_handler n
    .quad exception_stub_\n
.endm

.altmacro
exception_stub_table:
.set i, 0
.rept 32
    exception_handler %i
.set i, i+1
.endr