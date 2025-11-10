#ifndef VIPEROS_LIBC_STRING
#define VIPEROS_LIBC_STRING 1

#define NULL (0)

typedef unsigned long size_t;

void *memcpy(void *d, const void *s, size_t size);
void *memset(void *d, int ch, size_t size);

size_t strlen(const char *s);

#endif // VIPEROS_LIBC_STRING
