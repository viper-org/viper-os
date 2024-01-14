#include <cpu/asm.h>

namespace atheris
{
    namespace x64
    {
        namespace cpu
        {
            uint64_t ReadGS()
            {
                uint64_t result;
                asm volatile("mov %%gs:0, %0" : "=r"(result));
                return result;
            }

            void WriteMSR(MSR msr, uint64_t value)
            {
                asm volatile("wrmsr" :: "a"(value & 0xFFFFFFFF), "d"((value >> 32) & 0xFFFFFFFF), "c"(msr));
            }

            uint64_t ReadMSR(MSR msr)
            {
                uint64_t ret;
                asm volatile("rdmsr" : "=a"(ret) : "c"(msr));
                return ret;
            }
        }
    }
}