#include "driver/console.h"
#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>

int itoa(int64_t value, char *sp, int radix)
{
    char tmp[32];
    char *tp = tmp;
    int64_t i;
    int64_t v;

    int64_t sign = (radix == 10 && value < 0);    
    if (sign)
        v = -value;
    else
        v = (int64_t)value;

    while (v || tp == tmp)
    {
        i = v % radix;
        v /= radix;
        if (i < 10)
          *tp++ = i+'0';
        else
          *tp++ = i + 'A' - 10;
    }

    int64_t len = tp - tmp;

    if (sign) 
    {
        *sp++ = '-';
        len++;
    }

    while (tp > tmp)
        *sp++ = *--tp;

    return len;
}

void printf(const char* format, ...)
{
    va_list arg;
    va_start(arg, format);
    
    uint32_t color = 0xffffff;
    int i = 0;
    while(format[i])
    {
        if (format[i] == '%')
        {
            if (format[++i] == '%')
                echis::console::PutChar(format[i++], color, 0);
            else if (format[i] == 'd' || format[i] == 'i')
            {
                char buf[32];
                int64_t n = va_arg(arg, int64_t);
                int count = itoa(n, buf, 10);
                echis::console::PutString(buf, count, color, 0);
                i++;
            }
            else if(format[i] == 'x')
            {
                char buf[32] = "0x";
                int64_t n = va_arg(arg, int64_t);
                int count = itoa(n, buf + 2, 16);
                echis::console::PutString(buf, count + 2, color, 0);
                i++;
            }
            else if (format[i] == 'c')
            {
                int ch_int = va_arg(arg, int);
                char ch = (char)ch_int;
                echis::console::PutChar(ch, color, 0);
                i++;
            }
            else if (format[i] == 's')
            {
                char* str = va_arg(arg, char*);
                echis::console::Print(str, color, 0);
                i++;
            }
            else if(format[i] == '#')
            {
                uint32_t newColor = va_arg(arg, uint32_t);
                color = newColor;
                i++;
            }
        }
        else
        {
            echis::console::PutChar(format[i++], color, 0);
        }
    }

    va_end(arg);
}