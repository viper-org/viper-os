#include "fs/vfs.h"
#include "syscall/syscalls.h"
#include <string.h>

int sys_uname(struct utsname *buf)
{
    struct vnode *file = lookuppn("/etc/hostname");
    size_t sz = 64;
    file->fs->read(file, buf->nodename, &sz, 0);

    buf->domainname[0] = 0;

    strcpy(buf->sysname, "ViperOS");
    strcpy(buf->release, "0.1.0");
    strcpy(buf->version, "Kernel NSMP PREEMPT_RT");
    strcpy(buf->machine, "x86_64");

    return 0;
}
