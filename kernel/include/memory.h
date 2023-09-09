#ifndef VIPER_OS_MEMORY_H
#define VIPER_OS_MEMORY_H 1
#include <stddef.h>

void* operator new(size_t bytes);
void operator delete(void* address);
void operator delete(void* address, size_t bytes);

#endif