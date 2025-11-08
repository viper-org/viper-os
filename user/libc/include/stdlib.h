#ifndef VIPEROS_LIBC_STDLIB
#define VIPEROS_LIBC_STDLIB 1

typedef unsigned long size_t;

void *malloc(size_t size);

void free(void *ptr);

#endif // VIPEROS_LIBC_STDLIB
