#ifndef VIPER_DRIVERS_APIC_HPP
#define VIPER_DRIVERS_APIC_HPP
#include <stdint.h>

namespace APIC
{
    constexpr int IOAPICID = 0;
    constexpr int IOAPICVER = 1;
    constexpr int IOAPICARB = 2;

    struct [[gnu::packed]] IOREDTBL
    {
        uint64_t vector : 8;
        uint64_t delivery : 3;
        uint64_t destMode : 1;
        uint64_t deliveryStatus : 1;
        uint64_t pinPolarity : 1;
        uint64_t remotIRR : 1;
        uint64_t triggerMode : 1;
        uint64_t mask : 1;
        uint64_t reserved : 39;
        uint64_t dest : 8;
    };

    void WriteRegister(int reg, uint32_t value);
    uint32_t ReadRegister(int reg);

    void WriteIOAPIC(int reg, uint32_t value);
    uint32_t ReadIOAPIC(int reg);

    void AddRedirEntry(int vector, int lapicVector, int cpu);

    void Init();
}

#endif // VIPER_DRIVERS_APIC_HPP