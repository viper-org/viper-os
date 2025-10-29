#ifndef VIPEROS_FS_TESTFS
#define VIPEROS_FS_TESTFS 1

#include "fs/vfs.h"

struct testfs_vfs
{
    struct vnode *root_node;
};

struct testfs_file
{
    char *name;
    char *data;
    size_t length;
};

struct testfs_dir
{
    char *name;
    struct vnode *contained;
};

void testfs_init(void);

#endif // VIPEROS_FS_TESTFS
