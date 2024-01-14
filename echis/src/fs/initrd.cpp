#include <fs/initrd.h>
#include <fs/vfs.h>

#include <atheris/fs/initrd.h>

#include <string.h>
#include <stdint.h>

namespace echis
{
    namespace fs
    {
        namespace initrd
        {
            struct TARHeader
            {
                char name[100];
                char mode[8];
                char uid[8];
                char gid[8];
                char size[12];
                char mtime[12];
                char chksum[8];
                char typeflag;
                char linkname[100];
                char magic[6];
                char version[2];
                char uname[32];
                char gname[32];
                char devmajor[8];
                char devminor[8];
                char prefix[155];
            };

            size_t ParseSize(const char* sz)
            {
                unsigned int size = 0;
                unsigned int j;
                unsigned int count = 1;
                for (j = 11; j > 0; j--, count *= 8)
                    size += ((sz[j - 1] - '0') * count);
                return size;
            }

            void Init()
            {
                void* address = atheris::fs::initrd::GetAddress();
                TARHeader* header = reinterpret_cast<TARHeader*>(address);
                while(!memcmp("ustar", header->magic, 5))
                {
                    char name[100] = "tmp:";
                    strcpy(name + 4, header->name);
                    vfs::create(name);
                    vfs::Node* node = vfs::lookup(name);
                    size_t size = ParseSize(header->size);
                    node->write(reinterpret_cast<char*>(header) + 512, size);

                    header = reinterpret_cast<TARHeader*>(reinterpret_cast<uint64_t>(header) + ((size + 511) / 512 + 1) * 512);
                }
            }
        }
    }
}