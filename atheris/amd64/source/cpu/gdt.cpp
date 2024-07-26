#include <cpu/gdt.h>

#include <echis/driver/debugcon.h>

namespace atheris
{
    namespace amd64
    {
        namespace cpu
        {
            namespace gdt
            {
                extern "C" void gdt_install(Pointer* gdtr);

                Descriptor::Descriptor(std::uint32_t base, std::uint16_t limit, std::uint8_t access, std::uint8_t flags)
                    : limit(limit)
                    , baseLow(base & 0xFFFF)
                    , baseMid((base >> 16) & 0xFF)
                    , access(access)
                    , flags(flags)
                    , baseHigh((base >> 24) & 0xFF)
                {
                }

                Descriptor table[3];
                Pointer gdtr;

                void Install()
                {
                    constexpr std::uint8_t CODE_ACCESS = 0x9A;
                    constexpr std::uint8_t DATA_ACCESS = 0x92;
                    constexpr std::uint8_t CODE_FLAGS  = 0xA0;
                    constexpr std::uint8_t DATA_FLAGS  = 0xC0;

                    table[0] = Descriptor(0, 0, 0, 0); // Null descriptor

                    table[1] = Descriptor(0, 0, CODE_ACCESS, CODE_FLAGS); // Kernel code
                    table[2] = Descriptor(0, 0, DATA_ACCESS, DATA_FLAGS); // Kernel data

                    gdtr = {
                        .limit = sizeof(table) - 1,
                        .base = table
                    };

                    gdt_install(&gdtr);

                    echis::driver::debugcon::Write("Installed GDT\n");
                }
            }
        }
    }
}