#ifndef VIPEROS_LD_SYSCALL
#define VIPEROS_LD_SYSCALL 1

struct stat
{
    long st_size;
};

static inline int stat(const char *path, struct stat *statbuf)
{
    int ret;
    __asm__ volatile("syscall" : "=a"(ret) : "a"(4), "D"(path), "S"(statbuf) : "r11", "rcx");
    return ret;
}

static inline void *mmap(unsigned long len)
{
    void *ret;
    __asm__ volatile("syscall" : "=a"(ret) : "a"(9), "S"(len) : "rcx", "r11");
    return ret;
}

static inline int open(const char *path, unsigned short mode)
{
    int ret;
    __asm__ volatile("syscall" : "=a"(ret) : "a"(2), "D"(path), "S"(mode) : "r11", "rcx");
    return ret;
}

static inline int close(int fd)
{
    int ret;
    __asm__ volatile("syscall" : "=a"(ret) : "a"(3), "D"(fd) : "r11", "rcx");
    return ret;
}

static inline long read(int fd, void *buf, unsigned long size)
{
    long ret;
    __asm__ volatile("syscall" : "=a"(ret) : "a"(0), "D"(fd), "S"(buf), "d"(size) : "r11", "rcx");
    return ret;
}

static inline void load_library(void *buf, struct elf_exec *o)
{
    __asm__ volatile("syscall" :: "a"(59), "D"(buf), "S"(o) : "r11", "rcx");
}

void print(char *s)
{
    __asm__ volatile("syscall" :: "a"(67), "D"(s) : "r11", "rcx");
}

void printp(void *s)
{
    __asm__ volatile("syscall" :: "a"(68), "D"(s) : "r11", "rcx");
}

#endif // VIPEROS_LD_SYSCALL
