#ifndef VIPEROS_LIBC_STDIO
#define VIPEROS_LIBC_STDIO 1

typedef unsigned long size_t;
typedef unsigned long fpos_t;
typedef struct FILE_internal FILE;

#define EOF -1

extern struct FILE_internal stdin_internal;
extern struct FILE_internal stdout_internal;
extern struct FILE_internal stderr_internal;

#define stdin  (&stdin_internal)
#define stdout (&stdout_internal)
#define stderr (&stderr_internal)

int fputs(const char *restrict str, FILE *restrict stream);
int fputc(int ch, FILE *stream);
int putc(int ch, FILE *stream);

size_t fwrite(const void *restrict buffer, size_t size, size_t count, FILE *restrict stream);


int puts(const char *str);

#endif // VIPEROS_LIBC_STDIO
