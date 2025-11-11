#include <stdlib.h>

#include "sys/mman.h"

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


void free(void *ptr)
{
    libc_free(ptr);
}
