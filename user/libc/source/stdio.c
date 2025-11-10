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
