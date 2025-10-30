#ifndef VIPEROS_SCHED_PIPE
#define VIPEROS_SCHED_PIPE 1

#include <stdint.h>

enum pipe_type
{
    PIPE_READ,
    PIPE_WRITE
};

struct poll_event_object;

struct pipe
{
    enum pipe_type type;
    int fd;

    union
    {
        struct
        {
            char *buf;
            size_t size;
            size_t cap;
        } write;

        struct
        {
            struct pipe *write_end;
        } read;
    };

    struct poll_event_object *poll_event;
};

ssize_t write_pipe(struct pipe *pipe, const void *buf, size_t count);
ssize_t read_pipe(struct pipe *pipe, void *buf, size_t count);

void make_pipe(int fds[2]);

#endif // VIPEROS_SCHED_PIPE
