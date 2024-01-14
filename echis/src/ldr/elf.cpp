#include <ldr/elf.h>

#include <mm/pmm.h>

#include <util/math.h>

#include <string.h>

namespace echis
{
    namespace elf
    {
        struct Elf64_Ehdr
        {
            unsigned char e_ident[16];
            uint16_t e_type;
            uint16_t e_machine;
            uint32_t e_version;
            uint64_t e_entry;
            uint64_t e_phoff;
            uint64_t e_shoff;
            uint32_t e_flags;
            uint16_t e_ehsize;
            uint16_t e_phentsize;
            uint16_t e_phnum;
            uint16_t e_shentsize;
            uint16_t e_shnum;
            uint16_t e_shstrndx;
        };

        #define PT_LOAD 1
        struct Elf64_Phdr
        {
            uint32_t p_type;
            uint32_t p_flags;
            uint64_t p_offset;
            uint64_t p_vaddr;
            uint64_t p_paddr;
            uint64_t p_filesz;
            uint64_t p_memsz;
            uint64_t p_align;
        };

        Executable Load(void* addr, atheris::vm::AddressSpace* addressSpace)
        {
            addressSpace->switchTo();

            char* buffer = reinterpret_cast<char*>(addr);
            Elf64_Ehdr* ehdr = reinterpret_cast<Elf64_Ehdr*>(buffer);
            Elf64_Phdr* phdr = reinterpret_cast<Elf64_Phdr*>(buffer + ehdr->e_phoff);

            for (uint32_t i = 0; i < ehdr->e_phnum; ++i, ++phdr)
            {
                if (phdr->p_type != PT_LOAD)
                {
                    continue;
                }

                uint64_t vaddr = phdr->p_vaddr + 0x400000;
                uint32_t size = util::AlignUp(phdr->p_memsz, pmm::GetPageSize());
                for (uint32_t j = 0; j < size; j += pmm::GetPageSize())
                {
                    atheris::vm::MapPage(addressSpace,
                                         pmm::GetPage(),
                                         vaddr + j,
                                         atheris::vm::flags::present | atheris::vm::flags::write | atheris::vm::flags::user);
                }
                char* fileOffset = buffer + phdr->p_offset;
                memcpy(reinterpret_cast<char*>(vaddr), fileOffset, phdr->p_filesz);
                char* bss = reinterpret_cast<char*>(vaddr) + phdr->p_filesz;
                memset(bss, 0, phdr->p_memsz - phdr->p_filesz);
            }

            return Executable {
                .entryPoint = ehdr->e_entry + 0x400000
            };
        }
    }
}