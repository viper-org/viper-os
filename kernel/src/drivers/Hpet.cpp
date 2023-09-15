#include <drivers/Hpet.h>

#include <mm/Paging.h>
#include <mm/Pmm.h>

namespace HPET
{
    uint8_t* address = nullptr;

    constexpr int CAPABILITIES = 0x00;
    constexpr int MAIN_COUNTER = 0xF0;

    void WriteRegister(int reg, uint64_t value)
    {
        *((uint64_t*)(address + reg)) = value;
    }

    uint64_t ReadRegister(int reg)
    {
        return *((uint64_t*)(address + reg));
    }

    void Init()
    {
        HPET* hpet = (HPET*)ACPI::FindTable("HPET");
        address = (uint8_t*)PMM::PhysToVirt(hpet->address.addr);
        paging::MapPage(nullptr, hpet->address.addr, (uint64_t)address, 0x3);
        
        WriteRegister(0x10, ReadRegister(0x10) | 1); // ENABLE_CNF = 1
    }

    void Sleep(uint64_t count)
    {
        uint64_t end = ReadRegister(MAIN_COUNTER) + count / (ReadRegister(CAPABILITIES) >> 32);
        while(ReadRegister(MAIN_COUNTER) < end);
    }
}