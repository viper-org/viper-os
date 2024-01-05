#ifndef VIPEROS_ATHERIS_X64_CPU_CONTEXT_H
#define VIPEROS_ATHERIS_X64_CPU_CONTEXT_H 1

#include <stdint.h>

namespace atheris
{
    namespace x64
    {
        namespace cpu
        {
            struct Context
            {
                struct {
                    uint64_t    cr4;
                    uint64_t    cr3;
                    uint64_t    cr2;
                    uint64_t    cr0;
                } ControlRegisters;

                struct {
                    uint64_t    r8;
                    uint64_t    r9;
                    uint64_t    r10;
                    uint64_t    r11;
                    uint64_t    r12;
                    uint64_t    r13;
                    uint64_t    r14;
                    uint64_t    r15;
                    uint64_t    rdi;
                    uint64_t    rsi;
                    uint64_t    rdx;
                    uint64_t    rcx;
                    uint64_t    rbx;
                    uint64_t    rax;
                } GeneralRegisters;
                
                struct {
                    uint64_t    rbp;
                    uint64_t    vector;
                    uint64_t    error_code;
                    uint64_t    rip;
                    uint64_t    cs;
                    uint64_t    rflags;
                    uint64_t    rsp;
                    uint64_t    dss;
                } BaseFrame;
            };
        }
    }
}

#endif // VIPEROS_ATHERIS_X64_CPU_CONTEXT_H