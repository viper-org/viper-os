.text
.global install_gdt
.type install_gdt, @function
install_gdt:
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
    movw %ax, %ss
    movw %ax, %fs
    movw %ax, %gs
    ret
