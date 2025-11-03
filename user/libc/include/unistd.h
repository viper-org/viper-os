#ifndef VIPEROS_LIBC_UNISTD
#define VIPEROS_LIBC_UNISTD

#ifdef __cplusplus
extern "C" {
#endif


typedef unsigned long size_t;
typedef long ssize_t;
typedef int pid_t;
typedef long off_t;


enum
{
    O_RDONLY = 1 << 0,
    O_WRONLY = 1 << 1,
    O_RDWR   = O_RDONLY | O_WRONLY,
    
    O_CREAT  = 1 << 2,
    O_APPEND = 1 << 3
};

#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2

struct stat
{
    size_t st_size;
};

struct elf_exec
{
    unsigned long entry;

    unsigned long at_entry;
    unsigned long at_phdr;
    unsigned long at_phent;
    unsigned long at_phnum;
    unsigned long at_base1;
};

ssize_t read(int fd, void *buf, size_t count);
ssize_t write(int fd, const void *buf, size_t count);
int open(const char *path, int flags);
int close(int fd);
off_t lseek(int fd, off_t offset, int whence);
int pipe(int pipefd[2]);
pid_t spawn(const char *path);
int dup(int fd);
int dup2(int oldfd, int newfd);
pid_t getpid(void);
// getppid
void loadlib(const char *path, struct elf_exec *out);
__attribute__((noreturn)) void _exit(int status);


#ifdef __cplusplus
}
#endif

#endif // VIPEROS_LIBC_UNISTD
