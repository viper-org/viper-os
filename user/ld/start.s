.data
.globl initial_stack
initial_stack: .quad 0

.text
.extern ld_start_main
.globl _start
.type _start,@function
_start:
    mov %rsp, initial_stack(%rip)
    sub -8(%rsp), %rsp
    call ld_start_main
    jmp .
