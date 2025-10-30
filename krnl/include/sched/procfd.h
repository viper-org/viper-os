#ifndef VIPEROS_SCHED_PROCFD
#define VIPEROS_SCHED_PROCFD 1

#include "fs/vfs.h"

enum openmode
{
    OMODE_R = 1 << 0,
    OMODE_W = 1 << 1,

    O_CREAT = 1 << 2,
};

struct proc_fd
{
    struct vnode *vnode;
    uint8_t flags;
    size_t seek;
};

#endif // VIPEROS_SCHED_PROCFD
