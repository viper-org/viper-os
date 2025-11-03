#ifndef VIPEROS_ACPI_ACPI
#define VIPEROS_ACPI_ACPI 1

#include <stdint.h>

struct __attribute__((packed)) rsdp
{
    char signature[8];
    uint8_t checksum;
    char OEMID[6];
    uint8_t revision;
    uint32_t rsdt;
};

struct __attribute__((packed)) rsdp2
{
    struct rsdp first;

    uint32_t length;
    uint64_t xsdt;
    uint8_t checksum;
    uint8_t reserved[3];
};

struct __attribute__((packed)) sdt_header
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

struct __attribute__((packed)) xsdt
{
    struct sdt_header header;
};

struct __attribute__((packed)) rsdt
{
    struct sdt_header header;
    uint32_t other[];
};

void acpi_init(void);
void *acpi_find_table(const char *sig);

#endif // VIPEROS_ACPI_ACPI
