#include "sched/pipe.h"
#include "mm/kheap.h"
#include "sched/proc.h"
#include "sched/sched.h"
#include <string.h>

ssize_t write_pipe(struct pipe *pipe, const void *buf, size_t count)
{
    if (pipe->type != PIPE_WRITE) return -1;

    if (pipe->write.size + count > pipe->write.cap)
    {
        char *new_buf = kheap_alloc(pipe->write.size + count);
        if (pipe->write.buf)
        {
            memcpy(new_buf, pipe->write.buf, pipe->write.size + count);
            kheap_free(pipe->write.buf);
        }
        pipe->write.buf = new_buf;
        pipe->write.cap = pipe->write.size + count;
    }

    memcpy(pipe->write.buf + pipe->write.size, buf, count);
    pipe->write.size += count;
    return count;
}

ssize_t read_pipe(struct pipe *pipe, void *buf, size_t count)
{
    if (pipe->read.write_end->write.size < count)
    {
        count = pipe->read.write_end->write.size;
    }
    if (!pipe->read.write_end->write.buf) return -1;

    memcpy(buf, pipe->read.write_end->write.buf, count);
    pipe->read.write_end->write.size -= count;

    for (size_t i = 0; i < pipe->read.write_end->write.size; ++i)
    {
        pipe->read.write_end->write.buf[i] = pipe->read.write_end->write.buf[i + count];
    }
    return count;
}

void make_pipe(int fds[2])
{
    struct pipe *write = kheap_alloc(sizeof (struct pipe));
    write->type = PIPE_WRITE;
    memset(&write->write, 0, sizeof write->write);

    struct pipe *read = kheap_alloc(sizeof (struct pipe));
    read->type = PIPE_READ;
    read->read.write_end = write;
    
    struct process *proc = sched_curr()->owner;
    fds[0] = proc_add_pipefd(proc, read);
    fds[1] = proc_add_pipefd(proc, write);
}
