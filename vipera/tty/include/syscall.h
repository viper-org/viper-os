#ifndef VIPEROS_VIPERA_TTY_SYSCALL_H
#define VIPEROS_VIPERA_TTY_SYSCALL_H 1

typedef unsigned long size_t;

#define READ  1
#define WRITE 2

int open(const char* path, int flags);
size_t read(int fd, void* buf, size_t count);
size_t write(int fd, const void* buf, size_t count);
int close(int fd);
int ioctl(int fd, unsigned long request, void* arg);
void* mmap(size_t length);
size_t seek(int fd, size_t offset);

#endif // VIPEROS_VIPERA_TTY_SYSCALL_H