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
    sub $8, %rsp
    call ld_start_main
    jmp .

.extern resolve_sym
.globl resolve_sym_i
.type resolve_sym_i,@function
resolve_sym_i:
    pop %rdi
    pop %rsi
    call resolve_sym
    jmp *%rax
