#ifndef VIPEROS_CPU_INT_IOAPIC
#define VIPEROS_CPU_INT_IOAPIC 1

#include <stdint.h>

union redir_entry
{
    struct
    {
        uint64_t vector : 8;
        uint64_t delv_mode : 3;
        uint64_t dest_mode : 1;
        uint64_t delv_status : 1;
        uint64_t pin_polarity : 1;
        uint64_t remote_irr : 1;
        uint64_t trigger_mode : 1;
        uint64_t mask : 1;
        uint64_t reserved : 39;
        uint64_t destination : 8;
    };
    struct
    {
        uint32_t low;
        uint32_t high;
    };
};

void ioapic_init(void);

void ioapic_add_redir(uint8_t no, union redir_entry ent);

#endif // VIPEROS_CPU_INT_IOAPIC
