#ifndef VIPEROS_LIBC_STRING
#define VIPEROS_LIBC_STRING 1

#define NULL (0)

typedef unsigned long size_t;

void *memcpy(void *d, const void *s, size_t size);
void *memset(void *d, int ch, size_t size);

int strcmp(const char *s1, const char *s2);
char *strcpy(char *d, const char *s);
char *strcat(char *d, const char *s);
size_t strlen(const char *s);
char *strtok(char *str, const char *delim);
char *strtok_s(char *str, const char *delim, char **buffer);
char *strchr(const char *str, int ch);
size_t strspn(const char *dest, const char *src);
size_t strcspn(const char *dest, const char *src);

#endif // VIPEROS_LIBC_STRING
