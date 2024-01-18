#ifndef VIPEROS_VIPERA_TTY_STRING_H
#define VIPEROS_VIPERA_TTY_STRING_H 1

typedef unsigned long size_t;

#define NULL ((void*)0)

void* memset(void* s, int c, size_t n);
void* memcpy(void *dest, const void *src, size_t count);

#endif // VIPEROS_VIPERA_TTY_STRING_H