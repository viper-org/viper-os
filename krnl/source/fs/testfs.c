#include "fs/testfs.h"
#include "fs/vfs.h"

#include "mm/kheap.h"

#include "syscall/stat.h"

#include <string.h>

struct testfs_vfs fs;
struct vfilesystem vfs;

static enum vfs_error testfs_read(struct vnode *, void *, size_t*, size_t);
static enum vfs_error testfs_write(struct vnode *, const void *, size_t, size_t);
static enum vfs_error testfs_stat(struct vnode *, struct stat *);
static enum vfs_error testfs_lookup(struct vnode *, char *, struct vnode **);
static enum vfs_error testfs_create(struct vnode *, char *, struct vnode **);
static enum vfs_error testfs_mkdir(struct vnode *, char *, struct vnode **);
static enum vfs_error testfs_ioctl(struct vnode *, unsigned long, void *);

static int testfs_fs_root(struct vnode **);
static int testfs_fs_mount(char *);

static struct vnode *testfs_create_dir(char *name)
{
    struct testfs_dir *tfs_dir = kheap_alloc(sizeof(struct testfs_dir));
    tfs_dir->contained = NULL;
    tfs_dir->name = strdup(name);

    struct vnode *node = kheap_alloc(sizeof(struct vnode));
    node->impl = tfs_dir;
    node->mounted = NULL;
    node->fs = &vfs;
    node->type = VNODE_DIR;
    
    return node;
}

static struct vnode *testfs_create_file(char *name)
{
    struct testfs_file *tfs_file = kheap_alloc(sizeof(struct testfs_file));
    tfs_file->name = strdup(name);
    tfs_file->data = NULL;
    tfs_file->length = 0;

    struct vnode *node = kheap_alloc(sizeof(struct vnode));
    node->impl = tfs_file;
    node->mounted = NULL;
    node->fs = &vfs;
    node->type = VNODE_FILE;
    
    return node;
}

void testfs_init(void)
{
    fs.root_node = testfs_create_dir("/");
    
    vfs.impl = &fs;
    vfs.next = NULL;
    vfs.covered = NULL;

    vfs.root = testfs_fs_root;
    vfs.mount = testfs_fs_mount;

    vfs.read = testfs_read;
    vfs.write = testfs_write;
    vfs.stat = testfs_stat;
    vfs.lookup = testfs_lookup;
    vfs.create = testfs_create;
    vfs.mkdir = testfs_mkdir;
    vfs.ioctl = testfs_ioctl;

    pushvfs(&vfs);
    setrootfs(&vfs);
}


static enum vfs_error testfs_read(struct vnode *node, void *data, size_t* sz, size_t seek)
{
    if (node->type == VNODE_DIR) return VFS_IS_DIR;

    struct testfs_file *file = node->impl;
    if (*sz > file->length - seek)
    {
        *sz = file->length - seek;
    }
    memcpy(data, file->data + seek, *sz);
    return VFS_SUCCESS;
}

static enum vfs_error testfs_write(struct vnode *node, const void *data, size_t sz, size_t seek)
{
    if (node->type == VNODE_DIR) return VFS_IS_DIR;

    struct testfs_file *file = node->impl;
    char *olddata = file->data;
    
    file->data = kheap_alloc(sz);
    memcpy(file->data, olddata, seek);
    memcpy(file->data + seek, data, sz);
    file->length = sz;

    kheap_free(olddata);
    return VFS_SUCCESS;
}

static enum vfs_error testfs_stat(struct vnode *node, struct stat *statbuf)
{
    if (node->type == VNODE_DIR)
    {
        statbuf->st_size = 0; // todo: get dir size
    }
    else
    {
        struct testfs_file *file = node->impl;
        statbuf->st_size = file->length;
    }
    return VFS_SUCCESS;
}

static enum vfs_error testfs_lookup(struct vnode *node, char *name, struct vnode **out)
{
    if (node->type == VNODE_FILE) return VFS_IS_FILE;

    struct testfs_dir *dir = node->impl;
    struct vnode *curr = dir->contained;
    while (curr)
    {
        if (curr->type == VNODE_FILE)
        {
            struct testfs_file *file = curr->impl;
            if (!strcmp(file->name, name))
            {
                *out = curr;
                return VFS_SUCCESS;
            }
        }
        else if (curr->type == VNODE_DIR)
        {
            struct testfs_dir *dir = curr->impl;
            if (!strcmp(dir->name, name))
            {
                *out = curr;
                return VFS_SUCCESS;
            }
        }
        curr = curr->next;
    }
    return VFS_NOT_FOUND;
}

static enum vfs_error testfs_create(struct vnode *node, char *name, struct vnode **out)
{
    if (node->type == VNODE_FILE) return VFS_IS_FILE;

    struct testfs_dir *dir = node->impl;
    struct vnode *vnode = testfs_create_file(name);

    vnode->next = dir->contained;
    dir->contained = vnode;
    *out = vnode;
    return VFS_SUCCESS;
}

static enum vfs_error testfs_mkdir(struct vnode *node, char *name, struct vnode **out)
{
    if (node->type == VNODE_FILE) return VFS_IS_FILE;

    struct testfs_dir *dir = node->impl;
    struct vnode *vnode = testfs_create_dir(name);

    vnode->next = dir->contained;
    dir->contained = vnode;
    *out = vnode;
    return VFS_SUCCESS;
}

static enum vfs_error testfs_ioctl(struct vnode *n, unsigned long op, void *argp)
{
    return VFS_NOTTY;
}


static int testfs_fs_root(struct vnode **out)
{
    *out = fs.root_node;
    return 0;
}

static int testfs_fs_mount(char *path)
{
    return defaultmount(&vfs, path);
}
