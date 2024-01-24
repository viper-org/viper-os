.text
.extern libc_start_main
.extern _exit
.globl _start
_start:
    xor %rbp, %rbp
    # TODO: argc, argv
    xor %rax, %rax
    call libc_start_main

    mov %eax, %edi
    xor %rax, %rax
    call _exit
