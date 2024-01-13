.text
.globl test_user_program
.globl test_user_program_end
test_user_program:
    leaq msg(%rip), %rdi
    syscall
    jmp .

msg: .asciz "Hello From Userspace!\n"
test_user_program_end:
