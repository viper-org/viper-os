#include <stdlib.h>

#include <cstddef>
#include <cstdint>

void strrev(char* str, std::size_t len)
{
    std::size_t beg = 0;
    std::size_t end = len - 1;
    while (beg < end)
    {
        char tmp = str[beg];
        str[beg] = str[end];
        str[end] = tmp;
        --end;
        ++beg;
    }
}

template <class T>
char* IntegralToAscii(T n, char* buf, int radix)
{
    if (n == 0)
    {
        buf[0] = '0';
        buf[1] = '\0';
        return buf;
    }

    bool negative = false;
    if (n < 0 && radix == 10)
    {
        negative = true;
        n = -n;
    }

    constexpr char charset[] = "0123456789ABCDEF";
    int idx = 0;
    while (n)
    {
        buf[idx++] = charset[n % radix];
        n /= radix;
    }
    if (negative) buf[idx++] = '-';
    buf[idx] = '\0';

    strrev(buf, idx);

    return buf;
}

char* itoa(int n, char* buf, int radix)
{
    return IntegralToAscii(n, buf, radix);
}

char* ptoa(void* p, char* buf, int radix)
{
    std::uintptr_t n = reinterpret_cast<std::uintptr_t>(p);
    return IntegralToAscii(n, buf, radix);
}