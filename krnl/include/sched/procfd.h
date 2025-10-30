#ifndef VIPEROS_SCHED_PROCFD
#define VIPEROS_SCHED_PROCFD 1

#include "fs/vfs.h"

enum openmode
{
    OMODE_R = 1 << 0,
    OMODE_W = 1 << 1,

    O_CREAT = 1 << 2,
    O_APPEND = 1 << 3,
};

struct pipe;
struct poll_event_object;

struct proc_fd
{
    struct vnode *vnode;
    struct pipe *pipe;
    uint16_t flags;
    size_t seek;

    struct poll_event_object *poll_event;
};

#endif // VIPEROS_SCHED_PROCFD
