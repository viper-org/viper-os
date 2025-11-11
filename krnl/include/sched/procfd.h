#ifndef VIPEROS_SCHED_PROCFD
#define VIPEROS_SCHED_PROCFD 1

#include "fs/vfs.h"

enum openmode
{
    OMODE_R = 1 << 0,
    OMODE_W = 1 << 1,

    O_CREAT = 1 << 2,
    O_APPEND = 1 << 3,

    O_DIRECTORY = 1 << 4,
};

struct pipe;

struct proc_fd
{
    struct vnode *vnode;
    struct pipe *pipe;
    uint16_t flags;
    size_t seek;

    struct proc_fd *enext; // for poll events
};

#endif // VIPEROS_SCHED_PROCFD
