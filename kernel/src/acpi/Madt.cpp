#include <acpi/Madt.h>

#include <mm/Pmm.h>
#include <mm/Paging.h>

namespace MADT
{
    MADT* madt = nullptr;

    uint32_t* lapic = nullptr;
    uint64_t ioapic = 0;
    uint8_t lapic_ids[64] = {0};
    struct
    {
        uint8_t irq;
        uint8_t gsi;
    } overrides[16] = {{0, 0}};
    uint8_t overrideCount = 0;
    uint8_t cores = 0;

    void Init()
    {
        madt = (MADT*)ACPI::FindTable("APIC");
        lapic = (uint32_t*)PMM::PhysToVirt(madt->lapic);

        paging::MapPage(nullptr, madt->lapic, madt->lapic + 0xFFFF800000000000, 3);

        uint8_t* start = (uint8_t*)madt;
        uint8_t* end = start + madt->header.length;
        uint8_t* offset = start + sizeof(MADT);

        while(offset < end)
        {
            uint8_t type = offset[0];
            uint8_t len = offset[1];
            switch(type)
            {
                case 0:
                {
                    lapic_ids[cores++] = offset[3];
                    break;
                }
                case 1:
                {
                    ioapic = ((uint32_t*)offset)[1];
                    paging::MapPage(nullptr, ioapic, ioapic + 0xFFFF800000000000, 3);
                    ioapic += 0xFFFF800000000000;
                    break;
                }
                case 2:
                {
                    overrides[overrideCount++] = {offset[3], offset[4]};
                    break;
                }
            }
            offset += len;
        }
    }

    uint32_t* GetLAPIC()
    {
        return lapic;
    }

    uint32_t* GetIOAPIC()
    {
        return (uint32_t*)ioapic;
    }

    uint8_t GetOverride(uint8_t irq)
    {
        for(int i = 0; i < overrideCount; i++)
        {
            if(overrides[i].irq == irq)
                return overrides[i].gsi;
        }
        return -1;
    }

    uint8_t* GetIDs()
    {
        return lapic_ids;
    }

    uint8_t GetNumCores()
    {
        return cores;
    }
}