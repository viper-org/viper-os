#ifndef VIPEROS_VIPERA_LIBC_SYS_MMAN_H
#define VIPEROS_VIPERA_LIBC_SYS_MMAN_H 1

#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned long size_t;

void *mmap(size_t length);

#ifdef __cplusplus
}
#endif

#endif // VIPEROS_VIPERA_LIBC_SYS_MMAN_H