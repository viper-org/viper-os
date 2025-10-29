#include "fs/initrd.h"
#include "driver/ldr/loader.h"
#include "fs/vfs.h"
#include "fs/devfs.h"
#include "fs/module.h"

#include <string.h>

struct tar_header
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

static inline size_t parse_size(const char *sz)
{
    unsigned int size = 0;
    unsigned int count = 1;
    for (int i = 11; i > 0; i--, count *= 8)
        size += ((sz[i - 1] - '0') * count);
    return size;
}

void initrd_init(void)
{
    struct vnode *root = lookuppn("/");
    struct vnode *tmp;
    root->fs->mkdir(root, "tmp", &tmp);

    void *addr = get_module("/initrd.tar")->address;
    struct tar_header *hdr = addr;
    while (!memcmp("ustar", hdr->magic, 5))
    {
        char name[100];
        strcpy(name, hdr->name);
        struct vnode *node;
        tmp->fs->create(tmp, name, &node);
        size_t sz = parse_size(hdr->size);
        node->fs->write(node, (char*)hdr + 512, sz);

        size_t namelen = strlen(name);
        if (!strcmp(name + namelen - 5, ".vdrv"))
        {
            struct driver drv = ldr_load((char*)hdr + 512);
            drv.init();
            devfs_add_drv(drv);
        }

        hdr = (struct tar_header *)((char *)hdr + ((sz + 511) / 512 + 1) * 512);
    }
}
