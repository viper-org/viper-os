#ifndef VIPER_OS_STRING_H
#define VIPER_OS_STRING_H 1

#include <cstddef>

void* memset(void* dest, int ch, size_t count);
int memcmp(const void* p1, const void* p2, size_t count);
void memcpy(void* dest, const void* src, size_t count);

int strcmp(const char* s1, const char* s2);
void strcpy(char* s1, const char* s2);
size_t strlen(const char* str);

#endif // VIPER_OS_STRING_H