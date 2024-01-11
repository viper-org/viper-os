#ifndef VIPEROS_ECHIS_STRING_H
#define VIPEROS_ECHIS_STRING_H 1

#include <stddef.h>

extern "C" int strcmp(const char* s1, const char* s2);

void memcpy(void* dest, const void* src, size_t count);
void* memset(void* dest, int ch, size_t count);
int memcmp(const void* a, const void* b, size_t count);

#endif