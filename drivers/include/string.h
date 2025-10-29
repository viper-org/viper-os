#ifndef VIPEROS_DRIVERS_STRING
#define VIPEROS_DRIVERS_STRING 1

typedef unsigned long size_t;

static inline void *memcpy(void *d, const void *s, size_t sz)
{
    char *a = d;
    const char *b = s;
    for (size_t i = 0; i < sz; ++i)
    {
        a[i] = b[i];
    }
    return d;
}

#endif // VIPEROS_DRIVERS_STRING
