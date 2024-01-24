#include <sys/mman.h>
#include <sys/unistd.h>

struct HEADER_struct
{
    size_t size;
    struct HEADER_struct *next;
};

struct HEADER_struct *fl;
#define HEAP_SZ 32

#define REMOVE_ENT(current, previous) do { \
    if (previous) previous->next = current->next;\
    else fl = current->next;\
} while(0)

void libc_alloc_init()
{
    fl = (struct HEADER_struct *)mmap(HEAP_SZ);
    fl->size = HEAP_SZ * sysconf(_SC_PAGE_SIZE);
    fl->next = NULL;
}

void *libc_alloc_bytes(size_t length)
{
    if(!length) return NULL;
    length += sizeof(struct HEADER_struct);

    struct HEADER_struct *previous = NULL;
    struct HEADER_struct *current = fl;

    while(current)
    {
        if(current->size >= length)
        {
            if (current->size == length || ((current->size - length) < sizeof(struct HEADER_struct)))
            {
                REMOVE_ENT(current, previous);
                return current + 1;
            }
            size_t newsz = current->size - length;
            void *newhdrloc = ((void *) current) + length;
            struct HEADER_struct *newhdr = (struct HEADER_struct *) newhdrloc;
            newhdr->size = newsz;

            REMOVE_ENT(current, previous);

            newhdr->next = fl;
            fl = newhdr;

            current->size = length;
            return current + 1;
        }
        previous = current;
        current = current->next;
    }

    return NULL;
}

void libc_free_bytes(void *ptr)
{
    if(!ptr) return;
    
    struct HEADER_struct *hdr = ((struct HEADER_struct *) ptr) - 1;
    hdr->next = fl;
    fl = hdr;
}