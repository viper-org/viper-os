[extern CommonExceptionHandler]

%macro ExceptionErrStub 1
ExceptionStub%+%1:
    push %1
    jmp ExceptionFrameAssembler
%endmacro

%macro ExceptionNoErrStub 1
ExceptionStub%+%1:
    push 0
    push %1
    jmp ExceptionFrameAssembler
%endmacro

ExceptionFrameAssembler:
    push rbp
    mov rbp, rsp

    push rax
    push rbx
    push rcx
    push rdx
    push rsi
    push rdi
    push r15
    push r14
    push r13
    push r12
    push r11
    push r10
    push r9
    push r8

    mov rax, cr0
    push rax
    mov rax, cr2
    push rax
    mov rax, cr3
    push rax
    mov rax, cr4
    push rax

    mov rdi, rsp
    call CommonExceptionHandler

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
    add rsp, 0x10

    iretq


    ExceptionErrStub    0
    ExceptionNoErrStub  1
    ExceptionNoErrStub  2
    ExceptionNoErrStub  3
    ExceptionNoErrStub  4
    ExceptionNoErrStub  5
    ExceptionNoErrStub  6
    ExceptionNoErrStub  7
    ExceptionErrStub    8
    ExceptionNoErrStub  9
    ExceptionErrStub    10
    ExceptionErrStub    11
    ExceptionErrStub    12
    ExceptionErrStub    13
    ExceptionErrStub    14
    ExceptionNoErrStub  15
    ExceptionNoErrStub  16
    ExceptionErrStub    17
    ExceptionNoErrStub  18
    ExceptionNoErrStub  19
    ExceptionNoErrStub  20
    ExceptionNoErrStub  21
    ExceptionNoErrStub  22
    ExceptionNoErrStub  23
    ExceptionNoErrStub  24
    ExceptionNoErrStub  25
    ExceptionNoErrStub  26
    ExceptionNoErrStub  27
    ExceptionNoErrStub  28
    ExceptionNoErrStub  29
    ExceptionErrStub    30
    ExceptionNoErrStub  31
    
[global ExceptionStubTable]
ExceptionStubTable:
%assign i 0 
%rep 32 
    dq ExceptionStub%+i
%assign i i+1 
%endrep