#include <driver/acpi.h>

#include <atheris/mm/vm.h>

#include <limine.h>

#include <cstring>

static volatile limine_rsdp_request rsdpRequest = {
    .id = LIMINE_RSDP_REQUEST,
    .revision = 1,
    .response = nullptr,
};

namespace atheris
{
    namespace amd64
    {
        namespace acpi
        {
            XSDT* xsdt = nullptr;
            RSDT* rsdt = nullptr;

            void Init()
            {
                RSDP* rsdp = (RSDP*)rsdpRequest.response->address;
                if(rsdp->revision == 2)
                {
                    RSDP2* rsdp2 = (RSDP2*)rsdp;
                    xsdt = reinterpret_cast<XSDT*>(vm::GetVirtualAddress(rsdp2->xsdt));
                }
                else
                    rsdt = reinterpret_cast<RSDT*>(vm::GetVirtualAddress(rsdp->rsdt));
            }

            void* FindTable(const char* signature)
            {
                if(xsdt)
                {
                    int entryCount = (xsdt->header.length - sizeof(xsdt->header)) / 8;

                    for(int i = 0; i < entryCount; i++)
                    {
                        uint64_t* other = (uint64_t*)((uint8_t*)xsdt + sizeof(XSDT));
                        SDTHeader* hdr = reinterpret_cast<SDTHeader*>(vm::GetVirtualAddress(other[i]));

                        if(!std::memcmp(hdr->signature, signature, 4))
                        {
                            return (void*)hdr;
                        }
                    }
                }
                else if(rsdt)
                {
                    int entryCount = (rsdt->header.length - sizeof(rsdt->header)) / 4;

                    for(int i = 0; i < entryCount; i++)
                    {
                        uint32_t* other = (uint32_t*)((uint8_t*)rsdt + sizeof(RSDT));
                        SDTHeader* hdr = reinterpret_cast<SDTHeader*>(vm::GetVirtualAddress(other[i]));
                        if(!std::memcmp((uint8_t*)hdr->signature, (uint8_t*)signature, 4))
                            return (void*)hdr;
                    }
                }
                return nullptr;
            }
        }
    }
}