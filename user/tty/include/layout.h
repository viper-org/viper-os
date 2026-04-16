#ifndef VIPEROS_TTY_LAYOUT
#define VIPEROS_TTY_LAYOUT 1

#include <stdint.h>

struct keyboard_layout {
    char name[8]; // should be plenty. i think all layouts are named something like "en-gb"
    uint32_t normal_scancode_map[128];
    uint32_t shifted_scancode_map[128];
};

#endif // VIPEROS_TTY_LAYOUT
