[global install_tss]
install_tss:
    mov ax, 0x18
    ltr ax
    ret