#ifndef VIPEROS_ACPI_MADT
#define VIPEROS_ACPI_MADT 1

#include "acpi/acpi.h"

#include <stdint.h>

struct __attribute__((packed)) madt
{
    struct sdt_header header;
    uint32_t lapic_address;
    uint32_t flags;
};

struct __attribute__((packed)) variable_record_header
{
    uint8_t type;
    uint8_t length;
    // more to follow but depends on type
};

struct __attribute__((packed)) ioapic_header
{
    struct variable_record_header header;
    uint8_t id;
    uint8_t reserved;
    uint32_t address;
    uint32_t gsi_base;
};

struct __attribute__((packed)) source_override_header
{
    struct variable_record_header header;
    uint8_t bus_source;
    uint8_t irq_soruce;
    uint32_t gsi;
    uint16_t flags;
};

void madt_init(void);
uint32_t madt_ioapic_base(void);
uint32_t madt_gsi_base(void);
uint32_t madt_get_gsi(uint8_t source);

#endif // VIPEROS_ACPI_MADT
