.text
.extern libc_stdio_init
.extern main
.extern _exit
.globl _start
.type _start,@function
_start:
    push %rdi
    push %rsi
    call libc_stdio_init
    pop %rsi
    pop %rdi
    call main
    mov %rax, %rdi
    mov $60, %rax
    call _exit