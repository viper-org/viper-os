#ifndef VIPEROS_LIBC_STDLIB
#define VIPEROS_LIBC_STDLIB 1

typedef unsigned long size_t;

#define NULL (0)

void *malloc(size_t size);

void free(void *ptr);

#endif // VIPEROS_LIBC_STDLIB
