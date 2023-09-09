#ifndef VIPER_OS_DRIVERS_TIMER_HPET_H
#define VIPER_OS_DRIVERS_TIMER_HPET_H 1

#include <acpi/Acpi.h>

namespace HPET
{
    struct [[gnu::packed]] AddrStructure
    {
        uint8_t addrspaceID;
        uint8_t regBitWidth;
        uint8_t regBitOff;
        uint8_t reserved;
        uint64_t addr;
    };

    struct [[gnu::packed]] HPET
    {
        ACPI::SDTHeader header;
        uint8_t hardwareRevID;
        uint8_t comparatorCount : 5;
        uint8_t counterSz : 1;
        uint8_t reserved : 1;
        uint8_t legacyReplace : 1;
        uint16_t pciVendorID;
        AddrStructure address;
        uint8_t hpetNo;
        uint16_t minTick;
        uint8_t pageProt;
    };

    void Init();

    void Sleep(uint64_t count);
}

#endif // VIPER_OS_DRIVERS_TIMER_HPET_H