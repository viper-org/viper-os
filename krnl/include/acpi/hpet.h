#ifndef VIPEROS_ACPI_HPET
#define VIPEROS_ACPI_HPET 1

#include "acpi/acpi.h"

#include <stdint.h>

struct __attribute__((packed)) addr_structure
{
    uint8_t addrspaceID;
    uint8_t regBitWidth;
    uint8_t regBitOff;
    uint8_t reserved;
    uint64_t addr;
};

struct __attribute__((packed)) hpet
{
    struct sdt_header header;
    uint8_t hardwareRevID;
    uint8_t comparatorCount : 5;
    uint8_t counterSz : 1;
    uint8_t reserved : 1;
    uint8_t legacyReplace : 1;
    uint16_t pciVendorID;
    struct addr_structure address;
    uint8_t hpetNo;
    uint16_t minTick;
    uint8_t pageProt;
};

void hpet_init(void);

void hpet_spin(uint64_t count);

#endif // VIPEROS_ACPI_HPET
