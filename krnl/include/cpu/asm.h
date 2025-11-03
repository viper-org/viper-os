#ifndef VIPEROS_CPU_ASM
#define VIPEROS_CPU_ASM 1

#include <stdint.h>

#define IA32_APIC_BASE 0x1B
#define IA32_EFER   0xC0000080
#define IA32_STAR   0xC0000081
#define IA32_LSTAR  0xC0000082
#define IA32_SFMASK 0xC0000084
#define GS_BASE     0xC0000101

static inline void wrmsr(uint64_t msr, uint64_t value)
{
    __asm__ volatile("wrmsr" :: "a"(value & 0xFFFFFFFF), "d"((value >> 32) & 0xFFFFFFFF), "c"(msr));
}

static inline uint64_t rdmsr(uint64_t msr)
{
    uint64_t ret;
    __asm__ volatile("rdmsr" : "=a"(ret) : "c"(msr));
    return ret;
}

#endif // VIPEROS_CPU_ASM
