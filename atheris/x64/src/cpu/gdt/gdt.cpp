#include <cpu/gdt/gdt.h>

namespace x64
{
    namespace cpu
    {
        namespace gdt
        {
            extern "C" void install_gdt(Pointer* gdtr);

            Descriptor::Descriptor(uint32_t base, uint16_t limit, uint8_t access, uint8_t flags)
                : limit(limit)
                , baseLow(base & 0xFFFF)
                , baseMid((base >> 16 & 0xFF))
                , access(access)
                , flags(flags)
                , baseHigh((base >> 24) & 0xFF)
            {
            }
            Descriptor::Descriptor() {}


            Descriptor gdt[3];
            Pointer gdtr;

            void Install()
            {
                constexpr uint8_t codeAccess = 0x9A;
                constexpr uint8_t dataAccess = 0x92;
                constexpr uint8_t codeFlags  = 0xA0;
                constexpr uint8_t dataFlags  = 0xC0;

                gdt[0] = Descriptor(0, 0, 0, 0); // Null descriptor

                gdt[1] = Descriptor(0, 0, codeAccess, codeFlags);
                gdt[2] = Descriptor(0, 0, dataAccess, dataFlags);

                gdtr = {
                    .limit = sizeof(gdt) - 1,
                    .base = gdt
                };

                install_gdt(&gdtr);
            }
        }
    }
}