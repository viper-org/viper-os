#include "driver/keyboard.h"
#include "driver/port.h"

#include "event/bus.h"

#include "event/object.h"
#include "fs/devfs.h"

#include "mm/kheap.h"
#include "mm/vm.h"

#include "sched/procfd.h"

#include "syscall/stat.h"

#include <string.h>

ssize_t kb_read(void *, size_t *, size_t);
ssize_t kb_write(const void *, size_t, size_t);
int kb_stat(struct stat *);
int kb_ioctl(unsigned long, char *);

static struct driver_header hdr = {
    .name = "kb",
    .read = kb_read,
    .write = kb_write,
    .stat = kb_stat,
    .ioctl = kb_ioctl,
};
static struct keyboard_event *kb_buf;
static struct keyboard_event *kb_buf_ptr;

struct driver_header *kb_get_driver(void)
{
    return &hdr;
}

void kb_init(void)
{
    kb_buf = vm_getpages(NULL, 2, 0, NULL);
    kb_buf_ptr = kb_buf;

    devfs_add_drv((struct driver) {&hdr, NULL});
}

void kb_handler(void)
{
    if ((char *)kb_buf_ptr - (char *)kb_buf >= 0x2000) return;
    int scancode = inb(0x60) & 0x7F;
    int type = inb(0x60) & 0x80;
    *kb_buf_ptr = (struct keyboard_event) {type, scancode};
    ++kb_buf_ptr;

    struct event_bus *bus = get_event_bus();
    struct poll_event_object *poll = bus->poll_events;
    while (poll)
    {
        struct poll_event_object *next = poll->next;
        struct proc_fd *curr = poll->desc;
        while (curr)
        {
            if (curr->vnode && curr->vnode->fs == devfs_get())
            {
                struct devfs_file *file = poll->desc->vnode->impl;
                if (file->drvr.header == &hdr)
                {
                    poll->out = curr;
                    ready_event(&poll->obj);
                    // todo: free event
                    break;
                }
            }
            curr = curr->enext;
        }
        poll = next;
    }
}

ssize_t kb_read(void *buf, size_t *count, size_t seek)
{
    (void)seek;
    if (*count > (char *)kb_buf_ptr - (char *)kb_buf)
    {
        *count = (char *)kb_buf_ptr - (char *)kb_buf;
    }
    if (!*count)
    {
        *count = 0;
        return 0;
    }

    memcpy(buf, kb_buf, *count);

    size_t count_events = *count / sizeof (struct keyboard_event);
    int remaining = kb_buf_ptr - kb_buf - count_events;
    for (int i = 0; i < remaining; ++i)
    {
        kb_buf[i] = kb_buf[i + count_events];
    }
    kb_buf_ptr -= count_events;

    return *count;
}

ssize_t kb_write(const void *buf, size_t count, size_t seek)
{
    (void)buf;
    (void)count;
    (void)seek;

    return -1;
}

int kb_stat(struct stat *statbuf)
{
    statbuf->st_size = (kb_buf_ptr - kb_buf) * sizeof(struct keyboard_event);
    return 0;
}

int kb_ioctl(unsigned long op, char *argp)
{
    (void)op;
    (void)argp;
    return -1;
}
