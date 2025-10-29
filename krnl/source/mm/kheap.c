#include "mm/kheap.h"
#include "mm/pm.h"
#include "mm/vm.h"

#include "util.h"

#include <stdint.h>

extern char _kernel_end[];

struct heap_header
{
    size_t size;
    struct heap_header *next;
};
static struct heap_header *fl;

#define HEAPSZ 32

void kheap_init(void)
{
    uint64_t firstPage = align_up((uint64_t)_kernel_end, 0x1000);
    for (uint32_t i = 0; i < HEAPSZ; ++i)
    {
        vm_map_page(
            NULL,
            pm_getpage(),
            firstPage + (i * 0x1000),
            PT_PRESENT | PT_WRITE
        );
    }
    fl = (struct heap_header *)firstPage;
    fl->size = HEAPSZ * 0x1000;
    fl->next = NULL;
}

static inline void fl_remove(struct heap_header *curr, struct heap_header *prev)
{
    if (prev) prev->next = curr->next;
    else fl = curr->next;

    curr->next = NULL;
}

static inline void get_more_pages(void)
{
    void* pages = vm_getpages(NULL, 32);
    struct heap_header *hdr = pages;
    hdr->next = fl;
    fl = hdr;
    hdr->size = 32 * 0x1000;
}

void *kheap_alloc(size_t sz)
{
    if (!sz) return NULL;

    sz += sizeof (struct heap_header);
    struct heap_header *curr = fl;
    struct heap_header *prev = NULL;

    while (curr)
    {
        if (curr->size >= sz)
        {
            if ((curr->size - sz) < sizeof(struct heap_header) + 1)
            {
                fl_remove(curr, prev);
                return curr + 1;
            }
            size_t newsz = curr->size - sz;
            char *newHdr = (char*)curr + sz;
            struct heap_header *hdr = (struct heap_header *)newHdr;
            hdr->size = newsz;
            
            fl_remove(curr, prev);
            hdr->next = fl;
            fl = hdr;
            
            curr->size = sz;
            return curr + 1;
        }
        prev = curr;
        curr = curr->next;
    }
    get_more_pages();
    return kheap_alloc(sz - sizeof(struct heap_header));
}

void kheap_free(void *mem)
{
    if (!mem) return;

    struct heap_header *hdr = (struct heap_header *)mem - 1;
    hdr->next = fl;
    fl = hdr;
}
