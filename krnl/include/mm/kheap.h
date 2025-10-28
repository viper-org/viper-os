#ifndef VIPEROS_MM_KHEAP
#define VIPEROS_MM_KHEAP 1

typedef unsigned long size_t;

void  kheap_init(void);
void *kheap_alloc(size_t);
void  kheap_free(void *mem);

#endif // VIPEROS_MM_KHEAP
