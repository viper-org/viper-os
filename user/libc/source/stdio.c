#include <stdio.h>

#include <stdlib.h>
#include <unistd.h>
#include <string.h>

struct FILE_internal
{
    int fd;
    fpos_t pos;
    int eof;
};

struct FILE_internal stdin_internal;
struct FILE_internal stdout_internal;
struct FILE_internal stderr_internal;

static inline void strrev(char *s, size_t l)
{
    size_t b = 0;
    size_t e = l - 1;
    while (b < e)
    {
        char t = s[b];
        s[b] = s[e];
        s[e] = t;
        --e;
        ++b;
    }
}

static inline char *itoa(unsigned long n, char *buf, int r)
{
    if (n == 0)
    {
        buf[0] = '0';
        buf[1] = '\0';
        return buf;
    }

    int negative = 0;
    if (n < 0 && r == 10)
    {
        negative = 1;
        n = -n;
    }

    static char charset[] = "0123456789ABCDEF";
    int idx = 0;
    while (n)
    {
        buf[idx++] = charset[n % r];
        n /= r;
    }
    if (negative) buf[idx++] = '-';
    buf[idx] = '\0';

    strrev(buf, idx);

    return buf;
}

void libc_stdio_init(void)
{
    stdin_internal =  (FILE){.fd = 0, .pos = 0, .eof = 0};
    stdout_internal = (FILE){.fd = 1, .pos = 0, .eof = 0};
    stderr_internal = (FILE){.fd = 1, .pos = 0, .eof = 0}; // todo: add proper stderr
}


static inline int get_flags(const char *mode)
{
    int flags = 0;
    if (!strcmp(mode, "r"))
    {
        flags = O_RDONLY;
    }
    else if (!strcmp(mode, "w"))
    {
        flags = O_WRONLY | O_CREAT;
    }
    else if (!strcmp(mode, "a"))
    {
        flags = O_WRONLY | O_APPEND | O_CREAT;
    }
    else if (!strcmp(mode, "r+"))
    {
        flags = O_RDWR;
    }
    else if (!strcmp(mode, "w+"))
    {
        flags = O_RDWR | O_CREAT;
    }
    else if (!strcmp(mode, "a+"))
    {
        flags = O_RDWR | O_APPEND | O_CREAT;
    }
    return flags;
}

FILE *fopen(const char *restrict filename, const char *restrict mode)
{
    int flags = get_flags(mode);
    int fd = open(filename, flags);
    if (fd < 0) return NULL;

    FILE *f = malloc(sizeof (struct FILE_internal));
    f->fd = fd;
    f->pos = 0;

    return f;
}

FILE *freopen(const char *restrict filename, const char *restrict mode, FILE *restrict stream)
{
    close(stream->fd);

    int flags = get_flags(mode);
    int fd = open(filename, flags);
    if (fd < 0)
    {
        free(stream);
        return NULL;
    }

    stream->fd = fd;
    stream->pos = 0;
    return stream;
}

int fclose(FILE *stream)
{
    int ret = close(stream->fd);
    free(stream);
    return ret;
}


int fputs(const char *restrict str, FILE *restrict stream)
{
    int ret = write(stream->fd, str, strlen(str));
    if (ret < 0)
    {
        stream->eof = 1;
        return EOF;
    }
    return ret;
    // todo: set error
}

int fputc(int ch, FILE *stream)
{
    unsigned char c = ch;
    int ret = write(stream->fd, &c, 1);
    if (ret < 0)
    {
        stream->eof = 1;
        return EOF;
    }
    stream->eof = 0;
    return ch;
}

int putc(int ch, FILE *stream)
{
    return fputc(ch, stream);
}


int fgetc(FILE *stream)
{
    char c;
    int res = read(stream->fd, &c, 1);
    if (res <= 0)
    {
        stream->eof = 1;
        return EOF;
    }
    stream->eof = 0;
    return c;
}

int getc(FILE *stream)
{
    return fgetc(stream);
}


size_t fwrite(const void *restrict buffer, size_t size, size_t count, FILE *restrict stream)
{
    int ret = write(stream->fd, buffer, size * count);
    if (ret < 0)
    {
        stream->eof = 1;
        return 0;
    }
    stream->eof = 0;
    return ret / size;
}

size_t fread(void *restrict buffer, size_t size, size_t count, FILE *restrict stream)
{
    int ret = read(stream->fd, buffer, size * count);
    if (ret < 0)
    {
        stream->eof = 1;
        return 0;
    }
    stream->eof = 0;
    return ret / size;
}


int puts(const char *str)
{
    int count = fputs(str, stdout);
    count += fputc('\n', stdout);
    return count;
}


int feof(FILE *stream)
{
    return stream->eof;
}


int printf(const char *restrict fmt, ...)
{
    va_list arg;
    va_start(arg, fmt);
    int ret = vprintf(fmt, arg);
    va_end(arg);
    return ret;
}

int fprintf(FILE *restrict stream, const char *restrict fmt, ...)
{
    va_list arg;
    va_start(arg, fmt);
    int ret = vfprintf(stream, fmt, arg);
    va_end(arg);
    return ret;
}

int vprintf(const char *restrict stream, va_list arg)
{
    return vfprintf(stdout, stream, arg);
}

int vfprintf(FILE *restrict stream, const char *restrict fmt, va_list arg)
{
    int written = 0;
    size_t i = 0;
    while(fmt[i])
    {
        if (fmt[i] == '%')
        {
            ++i;
            switch (fmt[i])
            {
                case '%':
                    fputc('%', stream);
                    ++written;
                    break;

                case 'c':
                {
                    char c = va_arg(arg, int);
                    fputc(c, stream);
                    ++written;
                    break;
                }

                case 's':
                {
                    char *s = va_arg(arg, char *);
                    written += fputs(s, stream);
                    break;
                }

                case 'i':
                case 'd':
                {
                    int n = va_arg(arg, int);
                    char buf[16];
                    written += fputs(itoa(n, buf, 10), stream);
                    break;
                }

                case 'p':
                {
                    void *p = va_arg(arg, void *);
                    char buf[19] = "0x";
                    written += fputs(itoa((unsigned long)p, buf+2, 16)-2, stream);
                    break;
                }
            }
        }
        else
        {
            putc(fmt[i], stream);
            ++written;
        }
        ++i;
    }
    return written;
}
