.text
.extern libc_stdio_init
.extern main
.extern _exit
.globl _start
.type _start,@function
_start:
    call libc_stdio_init
    call main
    mov %rax, %rdi
    mov $60, %rax
    call _exit