#include "driver/dbg.h"
#include "driver/port.h"
#include "driver/driver.h"

#include <stdarg.h>
#include <stdbool.h>

void dbg_writechar(char c)
{
    outb(0x3F8, c); // COM1
}

void dbg_print(const char *s)
{
    while(*s)
    {
        dbg_writechar(*s);
        ++s;
    }
}

void strrev(char *s, size_t l)
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

char *itoa(uint64_t n, char *buf, int r)
{
    if (n == 0)
    {
        buf[0] = '0';
        buf[1] = '\0';
        return buf;
    }

    bool negative = false;
    if (n < 0 && r == 10)
    {
        negative = true;
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

void dbg_printf_impl(const char *fmt, va_list arg)
{
    size_t i = 0;
    while(fmt[i])
    {
        if (fmt[i] == '%')
        {
            ++i;
            switch (fmt[i])
            {
                case '%':
                    dbg_writechar('%');
                    break;

                case 'c':
                {
                    char c = va_arg(arg, int);
                    dbg_writechar(c);
                    break;
                }

                case 's':
                {
                    char *s = va_arg(arg, char *);
                    dbg_print(s);
                    break;
                }

                case 'i':
                case 'd':
                {
                    int n = va_arg(arg, int);
                    char buf[16];
                    dbg_print(itoa(n, buf, 10));
                    break;
                }

                case 'p':
                {
                    void *p = va_arg(arg, void *);
                    char buf[19] = "0x";
                    dbg_print(itoa((uint64_t)p, buf+2, 16)-2);
                    break;
                }
            }
        }
        else
            dbg_writechar(fmt[i]);
        ++i;
    }
}

void dbg_printf(const char *fmt, ...)
{
    va_list arg;
    va_start(arg, fmt);
    dbg_printf_impl(fmt, arg);
    va_end(arg);
}

DRIVER_EXPORT void KeDebugLog(const char *s)
{
    dbg_print(s);
}

DRIVER_EXPORT void KeDebugLogFmt(const char *fmt, ...)
{
    va_list arg;
    va_start(arg, fmt);
    dbg_printf_impl(fmt, arg);
    va_end(arg);
}
