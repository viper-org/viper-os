#include "fs/devfs.h"
#include "fs/vfs.h"

#include "mm/kheap.h"

#include <string.h>

static struct devfs_vfs fs;
static struct vfilesystem vfs;

static enum vfs_error devfs_read(struct vnode *, void *, size_t);
static enum vfs_error devfs_write(struct vnode *, const void *, size_t);
static enum vfs_error devfs_lookup(struct vnode *, char *, struct vnode **);
static enum vfs_error devfs_create(struct vnode *, char *, struct vnode **);
static enum vfs_error devfs_mkdir(struct vnode *, char *, struct vnode **);
static enum vfs_error devfs_ioctl(struct vnode *, unsigned long, void *);

static int devfs_fs_root(struct vnode **);
static int devfs_fs_mount(char *);

static struct vnode *devfs_mkroot(void)
{
    struct devfs_root *root = kheap_alloc(sizeof(struct devfs_root));
    root->contained = NULL;

    struct vnode *node = kheap_alloc(sizeof(struct vnode));
    node->impl = root;
    node->fs = &vfs;
    node->mounted = NULL;
    node->type = VNODE_DIR;
    node->next = NULL;

    return node;
}

void devfs_init(void)
{
    fs.root_node = devfs_mkroot();
    vfs.impl = &fs;
    vfs.covered = NULL;
    vfs.next = NULL;

    vfs.root = devfs_fs_root;
    vfs.mount = devfs_fs_mount;

    vfs.read = devfs_read;
    vfs.write = devfs_write;
    vfs.lookup = devfs_lookup;
    vfs.create = devfs_create;
    vfs.mkdir = devfs_mkdir;
    vfs.ioctl = devfs_ioctl;

    pushvfs(&vfs);
}

struct vfilesystem *devfs_get(void)
{
    return &vfs;
}

void devfs_add_drv(struct driver drv)
{
    struct devfs_file *dev = kheap_alloc(sizeof(struct devfs_file));
    dev->drvr = drv;
    dev->name = strdup(drv.header->name);

    struct vnode *node = kheap_alloc(sizeof(struct vnode));
    node->impl = dev;
    node->fs = &vfs;
    node->mounted = NULL;
    node->type = VNODE_FILE;
    node->next = NULL;

    struct devfs_root *root = fs.root_node->impl;
    node->next = root->contained;
    root->contained = node;
}

static enum vfs_error devfs_read(struct vnode *node, void *data, size_t sz)
{
    if (node->type == VNODE_DIR) return VFS_IS_DIR;

    struct devfs_file *file = node->impl;
    file->drvr.header->read(data, sz);

    return VFS_SUCCESS; // TODO: output ssize_t
}

static enum vfs_error devfs_write(struct vnode *node, const void *data, size_t sz)
{
    if (node->type == VNODE_DIR) return VFS_IS_DIR;

    struct devfs_file *file = node->impl;
    file->drvr.header->write(data, sz);

    return VFS_SUCCESS; // TODO: output ssize_t
}

static enum vfs_error devfs_lookup(struct vnode *node, char *name, struct vnode **out)
{
    if (node->type == VNODE_FILE) return VFS_IS_FILE;

    struct devfs_root *root = node->impl;
    struct vnode *curr = root->contained;
    while (curr)
    {
        // assume file for now
        struct devfs_file *file = curr->impl;
        if (!strcmp(file->name, name))
        {
            *out = curr;
            return VFS_SUCCESS;
        }
        curr = curr->next;
    }
    return VFS_NOT_FOUND;
}

static enum vfs_error devfs_create(struct vnode *node, char *name, struct vnode **out)
{
    if (node->type == VNODE_FILE) return VFS_IS_FILE;
    return VFS_IS_DIR;
}

static enum vfs_error devfs_mkdir(struct vnode *node, char *name, struct vnode **out)
{
    if (node->type == VNODE_FILE) return VFS_IS_FILE;
    return VFS_IS_DIR;
}

static enum vfs_error devfs_ioctl(struct vnode *node, unsigned long op, void *argp)
{
    if (node->type == VNODE_DIR) return VFS_IS_DIR;

    struct devfs_file *file = node->impl;
    file->drvr.header->ioctl(op, argp);

    return VFS_SUCCESS; // TODO: output ssize_t
}


static int devfs_fs_root(struct vnode **out)
{
    *out = fs.root_node;
    return 0;
}

static int devfs_fs_mount(char *path)
{
    return defaultmount(&vfs, path);
}
