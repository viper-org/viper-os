#include <drivers/Apic.h>

#include <acpi/Madt.h>

#include <string.h>

namespace APIC
{
    void WriteRegister(int reg, uint32_t value)
    {
        uint8_t* lapic = (uint8_t*)MADT::GetLAPIC();
        *((uint32_t*)(lapic + reg)) = value;
    }

    uint32_t ReadRegister(int reg)
    {
        uint8_t* lapic = (uint8_t*)MADT::GetLAPIC();
        return *((uint32_t*)(lapic + reg));
    }

    void WriteIOAPIC(int reg, uint32_t value)
    {
        volatile uint32_t* ioapic = MADT::GetIOAPIC();
        ioapic[0] = reg & 0xFF;
        ioapic[4] = value;
    }

    uint32_t ReadIOAPIC(int reg)
    {
        volatile uint32_t* ioapic = MADT::GetIOAPIC();
        ioapic[0] = reg & 0xFF;
        return ioapic[4];
    }

    void AddRedirEntry(int vector, int lapicVector, int cpu)
    {
        uint64_t red;
        IOREDTBL redtbl = {
            (uint64_t)lapicVector, 0, 0, 0, 0, 0, 0, 0, 0, (uint64_t)cpu
        };
        memcpy(&red, &redtbl, sizeof(uint64_t));
        WriteIOAPIC(vector * 2 + 0x10, red & 0xFFFFFFFF);
        WriteIOAPIC(vector * 2 + 0x11, (red >> 32) & 0xFFFFFFFF);
    }

    void Init()
    {
        WriteRegister(0xF0, 0x1FF);
    }
}