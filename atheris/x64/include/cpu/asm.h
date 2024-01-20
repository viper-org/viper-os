#ifndef VIPEROS_ATHERIS_X64_CPU_ASM_H
#define VIPEROS_ATHERIS_X64_CPU_ASM_H 1

#include <stdint.h>

namespace atheris
{
    namespace x64
    {
        namespace cpu
        {
            template <int ControlRegisterNo>
            requires (ControlRegisterNo == 0 
                || ControlRegisterNo == 2
                || ControlRegisterNo == 3
                || ControlRegisterNo == 4)
            void WriteCR(uint64_t value)
            {
                if      (ControlRegisterNo == 0) asm volatile("mov %0, %%rax; mov %%rax, %%cr0" :: "m"(value));
                else if (ControlRegisterNo == 2) asm volatile("mov %0, %%rax; mov %%rax, %%cr2" :: "m"(value));
                else if (ControlRegisterNo == 3) asm volatile("mov %0, %%rax; mov %%rax, %%cr3" :: "m"(value));
                else if (ControlRegisterNo == 4) asm volatile("mov %0, %%rax; mov %%rax, %%cr4" :: "m"(value));
            }

            template <int ControlRegisterNo>
            requires (ControlRegisterNo == 0 
                || ControlRegisterNo == 2
                || ControlRegisterNo == 3
                || ControlRegisterNo == 4)
            uint64_t ReadCR()
            {
                uint64_t result = 0;
                
                if      (ControlRegisterNo == 0) asm volatile("mov %%cr0, %%rax; mov %%rax, %0" : "=m"(result));
                else if (ControlRegisterNo == 2) asm volatile("mov %%cr2, %%rax; mov %%rax, %0" : "=m"(result));
                else if (ControlRegisterNo == 3) asm volatile("mov %%cr3, %%rax; mov %%rax, %0" : "=m"(result));
                else if (ControlRegisterNo == 4) asm volatile("mov %%cr4, %%rax; mov %%rax, %0" : "=m"(result));

                return result;
            }

            uint64_t ReadGS();

            enum class MSR : uint32_t
            {
                IA32_APIC_BASE = 0x1B,

                IA32_EFER      = 0xC0000080,
                IA32_STAR      = 0xC0000081,
                IA32_LSTAR     = 0xC0000082,
                IA32_SFMASK    = 0xC0000084,
                GSBase         = 0xC0000101,
            };
            void WriteMSR(MSR msr, uint64_t value);
            uint64_t ReadMSR(MSR msr);
        }
    }
}

#endif