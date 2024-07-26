#ifndef VIPEROS_ECHIS_STDLIB_H
#define VIPEROS_ECHIS_STDLIB_H 1

#ifdef __cplusplus
extern "C" {
#endif

char* itoa(int n, char* buf, int radix);
char* ptoa(void* p, char* buf, int radix);

#ifdef __cplusplus
}
#endif

#endif // VIPEROS_ECHIS_STDLIB_H