#ifndef VIPEROS_ATHERIS_X64_CPU_SYSCALL_H
#define VIPEROS_ATHERIS_X64_CPU_SYSCALL_H 1

#include <stdint.h>

namespace atheris
{
    namespace x64
    {
        namespace cpu
        {
            struct SyscallFrame
            {
                uint64_t r8;
                uint64_t r9;
                uint64_t r10;
                uint64_t rdx;
                uint64_t rsi;
                uint64_t rdi;
                uint64_t rax;
            };

            void EnableSyscall();
        }
    }
}

#endif // VIPEROS_ATHERIS_X64_CPU_SYSCALL_H