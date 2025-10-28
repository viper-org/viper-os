#ifndef VIPEROS_STRING
#define VIPEROS_STRING 1

typedef unsigned long size_t;

void *memset(void *d, int ch, size_t sz);
void *memcpy(void *d, const void *s, size_t sz);

size_t strlen(char *s);
int strcmp(char *s1, char *s2);
char *strdup(char *s);

#endif // VIPEROS_STRING
