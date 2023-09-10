[global launch_kernel_task]

launch_kernel_task:
    mov rsp, rdi ; CPU context is passed as an argument

    pop rax
    mov cr4, rax
    pop rax
    mov cr3, rax
    pop rax
    mov cr2, rax
    pop rax
    mov cr0, rax

    pop r8
    pop r9
    pop r10
    pop r11
    pop r12
    pop r13
    pop r14
    pop r15
    pop rdi
    pop rsi
    pop rdx
    pop rcx
    pop rbx
    pop rax

    pop rbp
    add rsp, 0x10 ; Vector and error code
    iretq ; Iretq pops the remaining values