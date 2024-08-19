#ifndef VIPEROS_ATHERIS_AMD64_DRIVER_HPET_H
#define VIPEROS_ATHERIS_AMD64_DRIVER_HPET_H 1

#include <driver/acpi.h>

#include <cstdint>

namespace atheris
{
    namespace amd64
    {
        namespace hpet
        {
            struct [[gnu::packed]] AddrStructure
            {
                std::uint8_t addrspaceID;
                std::uint8_t regBitWidth;
                std::uint8_t regBitOff;
                std::uint8_t reserved;
                std::uint64_t addr;
            };

            struct [[gnu::packed]] HPET
            {
                acpi::SDTHeader header;
                std::uint8_t hardwareRevID;
                std::uint8_t comparatorCount : 5;
                std::uint8_t counterSz : 1;
                std::uint8_t reserved : 1;
                std::uint8_t legacyReplace : 1;
                std::uint16_t pciVendorID;
                AddrStructure address;
                std::uint8_t hpetNo;
                std::uint16_t minTick;
                std::uint8_t pageProt;
            };

            void Init();

            void Sleep(uint64_t count);
        }
    }
}

#endif // VIPEROS_ATHERIS_AMD64_DRIVER_HPET_H