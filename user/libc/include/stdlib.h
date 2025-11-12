#ifndef VIPEROS_LIBC_STDLIB
#define VIPEROS_LIBC_STDLIB 1

typedef unsigned long size_t;

#define NULL (0)


void *malloc(size_t size);
void *calloc(size_t nmemb, size_t size);
void *realloc(void *ptr, size_t size);

void free(void *ptr);


_Noreturn void abort(void);
_Noreturn void exit(int exit_code);
_Noreturn void quick_exit(int exit_code);
_Noreturn void _Exit(int exit_code);

#define EXIT_SUCCESS (0)
#define EXIT_FAILURE (1)


typedef struct div_t { int quot; int rem; } div_t;
typedef struct ldiv_t { long quot; long rem; } ldiv_t;
typedef struct lldiv_t { long long quot; long long rem; } lldiv_t;

int abs(int j);
long int labs(long int j);
long long int llabs(long long int j);

div_t div(int numer, int denom);
ldiv_t ldiv(long int numer, long int denom);
lldiv_t lldiv(long long int numer, long long int denom);


int atoi(const char *str);
long atol(const char *str);
long long atoll(const char *str);

#endif // VIPEROS_LIBC_STDLIB
