#include <limine.h>

limine_framebuffer_request fbReq = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 1
};

extern "C" void kernel_main()
{
    unsigned char* addr = (unsigned char*)fbReq.response->framebuffers[0]->address;
    addr[10 * 4 + 10 * fbReq.response->framebuffers[0]->pitch] = 0xff;
    asm("1: cli; hlt: jmp 1b"); 
}