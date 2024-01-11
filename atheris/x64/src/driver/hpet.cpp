#include "atheris/private/mm/vm.h"
#include <driver/hpet.h>

#include <mm/vm.h>
#include <mm/util.h>

namespace x64
{
    namespace hpet
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
            HPET* hpet = reinterpret_cast<HPET*>(acpi::FindTable("HPET"));
            address = reinterpret_cast<uint8_t*>(PhysToVirt(hpet->address.addr));
            atheris::vm::MapPage(nullptr,
                                 hpet->address.addr, 
                                 reinterpret_cast<uint64_t>(address), 
                                 atheris::vm::flags::present | atheris::vm::flags::write);
            
            WriteRegister(0x10, ReadRegister(0x10) | 1); // ENABLE_CNF = 1
        }

        void Sleep(uint64_t count)
        {
            uint64_t end = ReadRegister(MAIN_COUNTER) + count / (ReadRegister(CAPABILITIES) >> 32);
            while(ReadRegister(MAIN_COUNTER) < end);
        }
    }
}