.text
.globl gdt_install
.type gdt_install, @function
gdt_install:
    cli
    lgdt (%rdi)
    pushq $0x8
    leaq reload_segments, %rax
    pushq %rax
    lretq

reload_segments:
    movw $0x10, %ax
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %fs
    movw %ax, %gs
    movw %ax, %ss
    ret