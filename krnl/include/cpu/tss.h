#ifndef VIPEROS_CPU_TSS
#define VIPEROS_CPU_TSS 1

#include <stdint.h>

void tss_init(void);

void tss_set_rsp0(uint64_t rsp0);

#endif // VIPEROS_CPU_TSS
