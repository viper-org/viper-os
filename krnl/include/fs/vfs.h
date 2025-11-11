#ifndef VIPEROS_FS_VFS
#define VIPEROS_FS_VFS 1

#include <stdint.h>

enum vnode_type
{
    VNODE_FILE,
    VNODE_DIR
};

enum vfs_error
{
    VFS_SUCCESS=0,
    VFS_IS_DIR,
    VFS_IS_FILE,
    VFS_NOT_FOUND,
    VFS_IS_MOUNTED,
    VFS_NOTTY
};

struct dirent
{
    unsigned char d_type;
    char d_name[256];
};

enum dirent_type
{
    DT_DIR,
    DT_REG
};

struct vnode;
struct stat;

struct vfilesystem
{
    void *impl; // vfs-specific data

    struct vfilesystem *next;
    struct vnode *covered;

    // vfs ops
    int (*root)(struct vnode **);
    int (*mount)(char *path);

    // vnode ops
    enum vfs_error (*read)(struct vnode *,void *buf, size_t* count, size_t seek);
    enum vfs_error (*write)(struct vnode *,const void *buf, size_t count, size_t seek);
    enum vfs_error (*stat)(struct vnode *, struct stat *statbuf);
    enum vfs_error (*lookup)(struct vnode *,char *component, struct vnode **);
    enum vfs_error (*create)(struct vnode *,char *name, struct vnode **);
    enum vfs_error (*mkdir)(struct vnode *,char *name, struct vnode **);
    enum vfs_error (*ioctl)(struct vnode *, unsigned long op, void *argp);
};

struct vnode
{
    void *impl; // vfs-specific data

    struct vfilesystem *fs;
    struct vfilesystem *mounted;
    enum vnode_type type;

    struct vnode *next;
};

void pushvfs(struct vfilesystem *);
void setrootfs(struct vfilesystem *);
enum vfs_error defaultmount(struct vfilesystem *, char *path);
struct vnode *lookuppn(char *path);
struct vnode *recursive_create(const char *path, int n);

char *get_relpath(const char *path, const char *cwd);

#endif // VIPEROS_FS_VFS
