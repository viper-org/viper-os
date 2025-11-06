#ifndef VIPEROS_DRIVER_KEYBOARD
#define VIPEROS_DRIVER_KEYBOARD

#include "driver/ldr/loader.h"

struct driver_header *kb_get_driver(void);

struct keyboard_event
{
    uint8_t mode; // down, up, repeat
    uint8_t scancode;
};

void kb_init(void);
void kb_handler(void);

#endif // VIPEROS_DRIVER_KEYBOARD
