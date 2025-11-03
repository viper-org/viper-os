#ifndef VIPEROS_CPU_INT_LAPIC
#define VIPEROS_CPU_INT_LAPIC 1

#include <stdint.h>

struct __attribute__((packed)) icr
{
    uint8_t vector;
    uint64_t flags : 12;
    uint64_t reserved : 36;
    uint8_t destination;
};

void lapic_init(void);
void lapic_init_timer(void);
uint64_t lapic_phys_addr(void);
void lapic_eoi(void);

#endif // VIPEROS_CPU_INT_LAPIC
