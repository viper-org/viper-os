.text
.globl enter_signal_handler
.type enter_signal_handler, @function
enter_signal_handler:
    movq %rdi, %rcx
    movq $0x202, %r11
    movq %rsi, %rsp

    sysretq