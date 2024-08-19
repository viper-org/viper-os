#include <driver/ldr/loader.h>

#include <mm/physical.h>

#include <util/math.h>

#include <atheris/mm/vm.h>

#include <limine.h>

#include <cstring>

namespace echis
{
    namespace driver
    {
        namespace ldr
        {
            static volatile limine_kernel_file_request KernelFileRequest = {
                .id = LIMINE_KERNEL_FILE_REQUEST,
                .revision = 1,
                .response = nullptr
            };

            typedef uint16_t Elf64_Half;
            typedef uint64_t Elf64_Off;
            typedef uint64_t Elf64_Addr;
            typedef uint32_t Elf64_Word;
            typedef uint64_t Elf64_Xword;
            typedef int32_t  Elf64_Sword;
            typedef int64_t  Elf64_Sxword;

            typedef struct
            {
                uint8_t	   e_ident[16];
                Elf64_Half e_type;
                Elf64_Half e_machine;
                Elf64_Word e_version;
                Elf64_Addr e_entry;
                Elf64_Off  e_phoff;
                Elf64_Off  e_shoff;
                Elf64_Word e_flags;
                Elf64_Half e_ehsize;
                Elf64_Half e_phentsize;
                Elf64_Half e_phnum;
                Elf64_Half e_shentsize;
                Elf64_Half e_shnum;
                Elf64_Half e_shstrndx;
            } Elf64_Ehdr;

            typedef struct
            {
                Elf64_Word	p_type;
                Elf64_Word	p_flags;
                Elf64_Off	p_offset;
                Elf64_Addr	p_vaddr;
                Elf64_Addr	p_paddr;
                Elf64_Xword	p_filesz;
                Elf64_Xword	p_memsz;
                Elf64_Xword	p_align;
            } Elf64_Phdr;

            typedef struct
            {
                Elf64_Word sh_name;
                Elf64_Word sh_type;
                Elf64_Xword sh_flags;
                Elf64_Addr sh_addr;
                Elf64_Off  sh_offset;
                Elf64_Xword sh_size;
                Elf64_Word sh_link;
                Elf64_Word sh_info;
                Elf64_Xword sh_addralign;
                Elf64_Xword sh_entsize;
            } Elf64_Shdr;

            typedef struct
            {
                Elf64_Word	  st_name;
                unsigned char st_info;
                unsigned char st_other;
                Elf64_Half st_shndx;
                Elf64_Addr	  st_value;
                Elf64_Xword	  st_size;
            } Elf64_Sym;

            constexpr Elf64_Word PT_LOAD = 1;

            static Elf64_Ehdr* kernelEhdr;
            static Elf64_Shdr* symtab = nullptr;
            static char* strtab = nullptr;

            static inline Elf64_Shdr* section(Elf64_Ehdr* ehdr, int n)
            {
                Elf64_Shdr* shdrStart = reinterpret_cast<Elf64_Shdr*>(reinterpret_cast<char*>(ehdr) + ehdr->e_shoff);
                return shdrStart + n;
            }

            static inline char* getShstrtab(Elf64_Ehdr* ehdr)
            {
                return (char*)(ehdr) + section(ehdr, ehdr->e_shstrndx)->sh_offset;
            }
 
            void Init()
            {
                kernelEhdr = reinterpret_cast<Elf64_Ehdr*>(KernelFileRequest.response->kernel_file->address);
                Elf64_Shdr* shdrStart = reinterpret_cast<Elf64_Shdr*>(reinterpret_cast<std::uintptr_t>(kernelEhdr) + kernelEhdr->e_shoff);
                char* shstrtab = getShstrtab(kernelEhdr);

                for (int i = 0; i < kernelEhdr->e_shnum; ++i)
                {
                    if (!std::strcmp(shstrtab + shdrStart[i].sh_name, ".symtab"))
                    {
                        symtab = &shdrStart[i];
                    }
                    if (!std::strcmp(shstrtab + shdrStart[i].sh_name, ".strtab"))
                    {
                        strtab = (char*)kernelEhdr + shdrStart[i].sh_offset;
                    }
                }
            }

            extern "C" void* KernelFunctionGetter(const char* name);

            Driver LoadDriver(void* addr)
            {
                char* file = reinterpret_cast<char*>(addr);
                Elf64_Ehdr* ehdr = reinterpret_cast<Elf64_Ehdr*>(addr);
                Elf64_Phdr* phdr = reinterpret_cast<Elf64_Phdr*>(file + ehdr->e_phoff);

                for (std::uint32_t i = 0; i < ehdr->e_phnum; ++i, ++phdr)
                {
                    if (phdr->p_type != PT_LOAD)
                    {
                        continue;
                    }

                    std::uint64_t vaddr = phdr->p_vaddr;
                    std::uint32_t size = util::AlignUp(phdr->p_memsz, mm::physical::GetPageSize());
                    for (uint32_t j = 0; j < size; j += mm::physical::GetPageSize())
                    {
                        atheris::vm::MapPage(nullptr,
                            mm::physical::GetPage(),
                            vaddr + j,
                            atheris::vm::flags::present | atheris::vm::flags::write);
                    }

                    char* start = file + phdr->p_offset;
                    std::memcpy(reinterpret_cast<char*>(vaddr), start, phdr->p_filesz);
                    char* bss = reinterpret_cast<char*>(vaddr) + phdr->p_filesz;
                    std::memset(bss, 0, phdr->p_memsz - phdr->p_filesz);

                    // TODO: Mark the area as used in the VMM
                }

                Elf64_Shdr* shdr = reinterpret_cast<Elf64_Shdr*>(file + ehdr->e_shoff);
                char* shstrtab = getShstrtab(ehdr);
                DriverHeader* header;
                for (int i = 0; i < ehdr->e_shnum; ++i)
                {
                    if (!std::strcmp(shstrtab + shdr[i].sh_name, ".driver_header"))
                    {
                        header = reinterpret_cast<DriverHeader*>(shdr[i].sh_addr);
                        header->kernel_func_getter = &KernelFunctionGetter;
                    }
                }

                return Driver{
                    .header = header,
                    .init = reinterpret_cast<void(*)()>(ehdr->e_entry),
                };
            }

            extern "C" void* KernelFunctionGetter(const char* name)
            {
                Elf64_Sym* symbol = reinterpret_cast<Elf64_Sym*>((char*)kernelEhdr + symtab->sh_offset);

                for (std::uint64_t i = 0; i < symtab->sh_size / sizeof(Elf64_Sym); ++i)
                {
                    if (!std::strcmp(strtab + symbol[i].st_name, name))
                    {
                        return reinterpret_cast<void*>(symbol[i].st_value);
                    }
                }
                return (void*)0xCA11CA11CA11CA11;
            }
        }
    }
}