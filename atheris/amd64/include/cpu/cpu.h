#ifndef VIPEROS_ATHERIS_AMD64_CPU_CPU_H
#define VIPEROS_ATHERIS_AMD64_CPU_CPU_H 1

#include <cstdint>

namespace atheris
{
    namespace amd64
    {
        namespace cpu
        {
            struct State
            {
                struct {
                    std::uint64_t    cr4;
                    std::uint64_t    cr3;
                    std::uint64_t    cr2;
                    std::uint64_t    cr0;
                } ControlRegisters;

                struct {
                    std::uint64_t    r8;
                    std::uint64_t    r9;
                    std::uint64_t    r10;
                    std::uint64_t    r11;
                    std::uint64_t    r12;
                    std::uint64_t    r13;
                    std::uint64_t    r14;
                    std::uint64_t    r15;
                    std::uint64_t    rdi;
                    std::uint64_t    rsi;
                    std::uint64_t    rdx;
                    std::uint64_t    rcx;
                    std::uint64_t    rbx;
                    std::uint64_t    rax;
                } GeneralRegisters;
                
                struct {
                    std::uint64_t    rbp;
                    std::uint64_t    vector;
                    std::uint64_t    error_code;
                    std::uint64_t    rip;
                    std::uint64_t    cs;
                    std::uint64_t    rflags;
                    std::uint64_t    rsp;
                    std::uint64_t    dss;
                } BaseFrame;
            };
        }
    }
}

#endif // VIPEROS_ATHERIS_AMD64_CPU_CPU_H