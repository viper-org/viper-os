#ifndef VIPEROS_ATHERIS_X64_DRIVER_ACPI_H
#define VIPEROS_ATHERIS_X64_DRIVER_ACPI_H 1

#include <stdint.h>

namespace x64
{
    namespace acpi
    {
        struct [[gnu::packed]] RSDP
        {
            char signature[8];
            uint8_t checksum;
            char OEMID[6];
            uint8_t revision;
            uint32_t rsdt;
        };

        struct [[gnu::packed]] RSDP2
        {
            RSDP first;

            uint32_t length;
            uint64_t xsdt;
            uint8_t checksum;
            uint8_t reserved[3];
        };

        struct [[gnu::packed]] SDTHeader
        {
            char signature[4];
            uint32_t length;
            uint8_t revision;
            uint8_t checksum;
            char OEMID[6];
            char OEMTableID[8];
            uint32_t OEMRevision;
            uint32_t creatorID;
            uint32_t creatorRevision;
        };

        struct [[gnu::packed]] XSDT
        {
            SDTHeader header;
        };

        struct [[gnu::packed]] RSDT
        {
            SDTHeader header;
            uint32_t other[];
        };

        void Init();

        void* FindTable(const char* signature);
    }
}

#endif // VIPEROS_ATHERIS_X64_DRIVER_ACPI_H