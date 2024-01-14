#include <cpu/syscall.h>
#include <cpu/asm.h>

#include <stdio.h>

namespace atheris
{
    namespace x64
    {
        namespace cpu
        {
            constexpr int SCE = (1 << 0);

            extern "C" void syscall_handler();

            void EnableSyscall()
            {
                uint64_t efer = ReadMSR(MSR::IA32_EFER);
                efer |= SCE;
                WriteMSR(MSR::IA32_EFER,  efer);
                WriteMSR(MSR::IA32_STAR,  ((uint64_t)0x8 << 32) | ((uint64_t)0x13 << 48));
                WriteMSR(MSR::IA32_LSTAR, reinterpret_cast<uint64_t>(syscall_handler));
            }

            extern "C" void syscall_dispatcher(char* message)
            {
                printf("%#%s", 0x00ffff, message);
            }
        }
    }
}