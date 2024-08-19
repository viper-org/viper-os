#ifndef VIPEROS_ECHIS_STRING_H
#define VIPEROS_ECHIS_STRING_H 1

#ifdef __cplusplus
extern "C" {
#endif

typedef decltype(sizeof(0)) size_t;

void* memset(void* dest, int ch, size_t count);
void* memcpy(void* dest, const void* src, size_t count );

int memcmp(const void* lhs, const void* rhs, size_t count);

int strcmp(const char* lhs, const char* rhs);

size_t strlen(const char* s);

#ifdef __cplusplus
}
#endif

#endif // VIPEROS_ECHIS_STRING_H