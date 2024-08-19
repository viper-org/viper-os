#ifndef VIPEROS_ATHERIS_AMD64_DRIVER_ACPI_H
#define VIPEROS_ATHERIS_AMD64_DRIVER_ACPI_H 1

#include <cstdint>

namespace atheris
{
    namespace amd64
    {
        namespace acpi
        {
            struct [[gnu::packed]] RSDP
            {
                char signature[8];
                std::uint8_t checksum;
                char OEMID[6];
                std::uint8_t revision;
                std::uint32_t rsdt;
            };

            struct [[gnu::packed]] RSDP2
            {
                RSDP first;

                std::uint32_t length;
                std::uint64_t xsdt;
                std::uint8_t checksum;
                std::uint8_t reserved[3];
            };

            struct [[gnu::packed]] SDTHeader
            {
                char signature[4];
                std::uint32_t length;
                std::uint8_t revision;
                std::uint8_t checksum;
                char OEMID[6];
                char OEMTableID[8];
                std::uint32_t OEMRevision;
                std::uint32_t creatorID;
                std::uint32_t creatorRevision;
            };

            struct [[gnu::packed]] XSDT
            {
                SDTHeader header;
            };

            struct [[gnu::packed]] RSDT
            {
                SDTHeader header;
                std::uint32_t other[];
            };

            void Init();

            void* FindTable(const char* signature);
        }
    }
}

#endif // VIPEROS_ATHERIS_AMD64_DRIVER_ACPI_H