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
    push %rdi
    push %rsi
    push %rdx
    push %rcx
    push %r8
    push %r9
    mov 48(%rsp), %rdi
    mov 56(%rsp), %rsi
    call resolve_sym
    pop %r9
    pop %r8
    pop %rcx
    pop %rdx
    pop %rsi
    pop %rdi
    add $16, %rsp
    jmp *%rax
