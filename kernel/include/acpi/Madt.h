#ifndef VIPER_OS_ACPI_MADT_H
#define VIPER_OS_ACPI_MADT_H 1
#include <acpi/Acpi.h>

namespace MADT
{
    struct [[gnu::packed]] MADT
    {
        ACPI::SDTHeader header;
        uint32_t lapic;
        uint32_t flags;
    };

    void Init();

    uint32_t* GetLAPIC();
    uint32_t* GetIOAPIC();

    uint8_t GetOverride(uint8_t irq);

    uint8_t* GetIDs();
}

#endif // VIPER_OS_ACPI_MADT_H