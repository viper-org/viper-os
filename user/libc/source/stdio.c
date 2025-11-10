#include <stdio.h>

#include <unistd.h>
#include <string.h>

struct FILE_internal
{
    int fd;
    fpos_t pos;
};

struct FILE_internal stdin_internal;
struct FILE_internal stdout_internal;
struct FILE_internal stderr_internal;

void libc_stdio_init(void)
{
    stdin_internal =  (FILE){.fd = 0, .pos = 0};
    stdout_internal = (FILE){.fd = 1, .pos = 0};
    stderr_internal = (FILE){.fd = 1, .pos = 0}; // todo: add proper stderr
}


int fputs(const char *restrict str, FILE *restrict stream)
{
    return write(stream->fd, str, strlen(str));
    // todo: set error
}

int fputc(int ch, FILE *stream)
{
    unsigned char c = ch;
    int ret = write(stream->fd, &c, 1);
    if (ret < 0) return EOF;
    return ch;
}

int putc(int ch, FILE *stream)
{
    return fputc(ch, stream);
}


size_t fwrite(const void *restrict buffer, unsigned long size, unsigned long count, FILE *restrict stream)
{
    int ret = write(stream->fd, buffer, size * count);
    if (ret < 0) return 0;
    return ret / size;
}


int puts(const char *str)
{
    int count = fputs(str, stdout);
    count += fputc('\n', stdout);
    return count;
}
