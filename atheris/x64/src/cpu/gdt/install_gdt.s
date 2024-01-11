.text
.globl install_gdt
.globl install_tss
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

.type install_tss, @function
install_tss:
    movw $0x28, %ax
    ltr %ax
    ret