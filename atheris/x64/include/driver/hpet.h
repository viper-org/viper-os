#ifndef VIPEROS_ATHERIS_X64_DRIVER_HPET_H
#define VIPEROS_ATHERIS_X64_DRIVER_HPET_H 1

#include <driver/acpi.h>

#include <stdint.h>

namespace x64
{
    namespace hpet
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
            acpi::SDTHeader header;
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
}

#endif // VIPEROS_ATHERIS_X64_DRIVER_HPET_H