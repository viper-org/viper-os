#ifndef VIPEROS_FS_DEVFS
#define VIPEROS_FS_DEVFS 1

#include "fs/vfs.h"

#include "driver/ldr/loader.h"

struct devfs_vfs
{
    struct vnode *root_node;
};

struct devfs_file
{
    char *name;
    
    struct driver drvr;
};

struct devfs_root
{
    struct vnode *contained;
};

void devfs_init(void);
struct vfilesystem *devfs_get(void);
void devfs_add_drv(struct driver);

#endif // VIPEROS_FS_DEVFS
