#ifndef VIPEROS_STRING
#define VIPEROS_STRING 1

typedef unsigned long size_t;

void *memset(void *d, int ch, size_t sz);
void *memcpy(void *d, const void *s, size_t sz);
int memcmp(const void *a, const void *b, size_t sz);

size_t strlen(const char *s);
int strcmp(const char *s1, const char *s2);
char *strcpy(char *s1, const char *s2);
char *strdup(const char *s);
char *strrchr(const char *s, int ch);

#endif // VIPEROS_STRING
