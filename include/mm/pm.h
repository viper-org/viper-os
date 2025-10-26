#ifndef VIPEROS_MM_PM
#define VIPEROS_MM_PM 1

typedef unsigned long long physaddr_t;

void pm_init(void);

physaddr_t pm_getpage(void);
physaddr_t pm_getpages(int n);

void pm_freepage(physaddr_t);
void pm_freepages(physaddr_t, int n);

#endif // VIPEROS_MM_PM
