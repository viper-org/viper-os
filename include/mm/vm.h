#ifndef VIPEROS_MM_VM
#define VIPEROS_MM_VM 1

typedef unsigned long long physaddr_t;

void *vm_phystovirt(physaddr_t);

#endif // VIPEROS_MM_VM
