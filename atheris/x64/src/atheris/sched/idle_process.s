.text
.globl idle
.type idle, @function
idle:
    sti
1:
    hlt
    jmp 1b