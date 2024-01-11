#include <cpu/syscall.h>
#include <cpu/asm.h>

namespace x64
{
    namespace cpu
    {
        constexpr int SCE = (1 << 0);

        void EnableSyscall()
        {
            uint64_t efer = x64::cpu::ReadMSR(x64::cpu::MSR::IA32_EFER);
            efer |= SCE;
            x64::cpu::WriteMSR(x64::cpu::MSR::IA32_EFER, efer);
            x64::cpu::WriteMSR(x64::cpu::MSR::IA32_STAR, ((uint64_t)0x8 << 32) | ((uint64_t)0x13 << 48));
        }
    }
}