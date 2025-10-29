#ifndef VIPEROS_CPU_ASM
#define VIPEROS_CPU_ASM 1

#include <stdint.h>

enum MSR
{
    IA32_EFER = 0xC0000080
};

static inline void wrmsr(enum MSR msr, uint64_t value)
{
    __asm__ volatile("wrmsr" :: "a"(value & 0xFFFFFFFF), "d"((value >> 32) & 0xFFFFFFFF), "c"(msr));
}

static inline uint64_t rdmsr(enum MSR msr)
{
    uint64_t ret;
    __asm__ volatile("rdmsr" : "=a"(ret) : "c"(msr));
    return ret;
}

#endif // VIPEROS_CPU_ASM
