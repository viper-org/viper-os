#include <cpu/Gdt/Gdt.h>
#include <stdint.h>


namespace GlobalDescriptorTable
{
    struct [[gnu::packed]] GDTPointer
    {
        uint16_t limit;
        uint64_t base;
    };

    extern "C" void install_gdt(GDTPointer* gdtr);

    uint64_t descriptors[3];

    void Init()
    {
        descriptors[0] = 0x0000000000000000; // Null descriptor
        descriptors[1] = 0x00a09a0000000000; // Kernel code descriptor
        descriptors[2] = 0x00c0920000000000; // Kernel data descriptor

        GDTPointer gdtr = {
            .limit = sizeof(descriptors) - 1,
            .base = (uint64_t)&descriptors[0],
        };

        install_gdt(&gdtr);
    }
}