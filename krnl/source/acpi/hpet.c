#include "acpi/hpet.h"

#include "mm/vm.h"

uint8_t *address = NULL;

#define CAPABILITIES 0x00
#define MAIN_COUNTER 0xF0

void write_hpet(int reg, uint64_t value)
{
    *((uint64_t*)(address + reg)) = value;
}

uint64_t read_hpet(int reg)
{
    return *((uint64_t*)(address + reg));
}

void hpet_init(void)
{
    struct hpet *hpet = acpi_find_table("HPET");
    address = vm_phystovirt(hpet->address.addr);
    vm_map_page(NULL,
        hpet->address.addr, 
        (uint64_t)address, 
        PT_PRESENT | PT_WRITE);
    
    write_hpet(0x10, read_hpet(0x10) | 1); // ENABLE_CNF = 1
}

void hpet_spin(uint64_t count)
{
    uint64_t end = read_hpet(MAIN_COUNTER) + count / (read_hpet(CAPABILITIES) >> 32);
    while(read_hpet(MAIN_COUNTER) < end);
}
