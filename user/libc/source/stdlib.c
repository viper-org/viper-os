#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "sys/mman.h"
#include "unistd.h"

struct heap_header
{
    size_t size;
    struct heap_header *next;
};
static struct heap_header *fl = NULL;

static inline void fl_remove(struct heap_header *curr, struct heap_header *prev)
{
    if (prev) prev->next = curr->next;
    else fl = curr->next;

    curr->next = NULL;
}

static inline void get_more_pages(void)
{
    void* pages = mmap(NULL, 32 * 0x1000, 0, MAP_ANONYMOUS, 0, 0);
    struct heap_header *hdr = pages;
    hdr->next = fl;
    fl = hdr;
    hdr->size = 32 * 0x1000;
}

static inline void *libc_allocate(size_t size)
{
    if (!size) return NULL;

    size += sizeof (struct heap_header);
    struct heap_header *curr = fl;
    struct heap_header *prev = NULL;

    while (curr)
    {
        if (curr->size >= size)
        {
            if ((curr->size - size) < sizeof(struct heap_header) + 1)
            {
                fl_remove(curr, prev);
                return curr + 1;
            }
            size_t newsize = curr->size - size;
            char *newHdr = (char*)curr + size;
            struct heap_header *hdr = (struct heap_header *)newHdr;
            hdr->size = newsize;
            
            fl_remove(curr, prev);
            hdr->next = fl;
            fl = hdr;
            
            curr->size = size;
            return curr + 1;
        }
        prev = curr;
        curr = curr->next;
    }
    get_more_pages();
    return libc_allocate(size - sizeof(struct heap_header));
}

static inline void libc_free(void *mem)
{
    if (!mem) return;

    struct heap_header *hdr = (struct heap_header *)mem - 1;
    hdr->next = fl;
    fl = hdr;
}


void *malloc(size_t size)
{
    return libc_allocate(size);
}

void *calloc(size_t nmemb, size_t size)
{
    void *mem = libc_allocate(nmemb * size);
    memset(mem, 0, nmemb * size);
    return mem;
}

void *realloc(void *ptr, size_t size)
{
    // todo: check if there is a block immediately after to combine
    void *new = libc_allocate(size);
    size_t min = size;
    struct heap_header *old = (struct heap_header *)(ptr) - 1;
    if (old->size < min) min = old->size;

    memcpy(new, ptr, min);
    free(ptr);
    return new;
}

void free(void *ptr)
{
    libc_free(ptr);
}


void abort(void)
{
    _exit(1); // TODO: kill(SIGABRT)
}

// todo: cleanup and call atexits
void exit(int exit_code)
{
    _exit(exit_code);
}

void quick_exit(int exit_code)
{
    _exit(exit_code);
}

void _Exit(int exit_code)
{
    _exit(exit_code);
}



int abs(int j)
{
    return (j < 0) ? -j : j;
}

long int labs(long int j)
{
    return (j < 0) ? -j : j;
}

long long int llabs(long long int j)
{
    return (j < 0) ? -j : j;
}


div_t div(int numer, int denom)
{
    return (div_t) {
        .quot = numer / denom,
        .rem = numer % denom
    };
}

ldiv_t ldiv(long int numer, long int denom)
{
    return (ldiv_t) {
        .quot = numer / denom,
        .rem = numer % denom
    };
}

lldiv_t lldiv(long long int numer, long long int denom)
{
    return (lldiv_t) {
        .quot = numer / denom,
        .rem = numer % denom
    };
}


int atoi(const char *str)
{
    int ret = 0;
    int i = 0;
    while (isdigit(str[i]))
    {
        ret *= 10;
        ret += str[i] - '0';
        ++i;
    }
    return ret;
}
