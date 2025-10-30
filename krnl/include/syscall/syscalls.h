#ifndef VIPEROS_SYSCALL_SYSCALLS
#define VIPEROS_SYSCALL_SYSCALLS 1

#include <stdint.h>

int sys_open(const char *path, uint16_t openmode);

#endif // VIPEROS_SYSCALL_SYSCALLS
