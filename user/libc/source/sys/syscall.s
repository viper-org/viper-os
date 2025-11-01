.text
.globl syscall0
.type syscall0,@function
syscall0:
    movq %rdi, %rax
    syscall
    ret

.globl syscall1
.type syscall1,@function
syscall1:
    movq %rdi, %rax
    mov %rsi, %rdi
    syscall
    ret

.globl syscall2
.type syscall2,@function
syscall2:
    movq %rdi, %rax
    mov %rsi, %rdi
    mov %rdx, %rsi
    syscall
    ret

.globl syscall3
.type syscall3,@function
syscall3:
    movq %rdi, %rax
    mov %rsi, %rdi
    mov %rdx, %rsi
    mov %rcx, %rdx
    syscall
    ret

.globl syscall4
.type syscall4,@function
syscall4:
    movq %rdi, %rax
    mov %rsi, %rdi
    mov %rdx, %rsi
    mov %rcx, %rdx
    mov %r8, %r10
    syscall
    ret

.globl syscall5
.type syscall5,@function
syscall5:
    movq %rdi, %rax
    mov %rsi, %rdi
    mov %rdx, %rsi
    mov %rcx, %rdx
    mov %r8, %r10
    mov %r9, %r8
    syscall
    ret

.globl syscall6
.type syscall6,@function
syscall6:
    movq %rdi, %rax
    mov %rsi, %rdi
    mov %rdx, %rsi
    mov %rcx, %rdx
    mov %r8, %r10
    mov %r9, %r8
    mov (%rsp), %r9
    syscall
    ret