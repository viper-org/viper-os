#include "fs/vfs.h"

#include "mm/kheap.h"

#include <string.h>

struct vfilesystem *vfs_list = NULL;
struct vfilesystem *rootfs;

void pushvfs(struct vfilesystem *fs)
{
    fs->next = vfs_list;
    vfs_list = fs;
}

void setrootfs(struct vfilesystem *fs)
{
    rootfs = fs;
}

enum vfs_error defaultmount(struct vfilesystem *fs, char *path)
{
    struct vnode *node = lookuppn(path);
    if (!node || node->mounted) return VFS_IS_MOUNTED;
    
    fs->covered = node;
    node->mounted = fs;
    return VFS_SUCCESS;
}

struct vnode *lookuppn(char *path)
{
    struct vnode *root;
    rootfs->root(&root);

    struct vnode *curr = root;
    struct vfilesystem *currfs = rootfs;

    if (path[0] != '/') return NULL; // only absolute paths for now

    char buf[129];
    size_t idx = 1;
    int bufIdx = 0;
    size_t pathlen = strlen(path);
    while (idx < pathlen)
    {
        while (path[idx] != '/' && idx < pathlen)
        {
            buf[bufIdx] = path[idx];
            ++idx;
            ++bufIdx;
        }
        buf[bufIdx] = 0;

        enum vfs_error err = currfs->lookup(curr, buf, &curr);
        if (err != VFS_SUCCESS)
        {
            return NULL;
        }

        if (curr->mounted)
        {
            currfs = curr->mounted;
            currfs->root(&curr);
        }

        bufIdx = 0;
        ++idx;
    }

    return curr;
}

struct vnode *recursive_create(const char *pn, int n)
{
    const char *component = strrchr(pn, '/')+1;
    char *parent = strdup(pn);
    parent[component - pn] = '\0';
    if (component[0] == 0)
    {
        parent[component - pn - 1] = 0;
    }
    struct vnode *node = lookuppn(parent);
    if (!node)
    {
        node = recursive_create(parent, 1);
        if (component[0] == 0) return node;
    }
    struct vnode *out;
    if (n) node->fs->mkdir(node, component, &out);
    else node->fs->create(node, component, &out);
    kheap_free(parent);
    return out;
}
