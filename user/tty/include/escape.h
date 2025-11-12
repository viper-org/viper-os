#ifndef VIPEROS_TTY_ESCAPE
#define VIPEROS_TTY_ESCAPE 1

enum key_escape
{
    ESC_CTRL = 1,
    ESC_SHIFT,
    ESC_UP,
    ESC_LEFT,
    ESC_DOWN,
    ESC_RIGHT,
    ESC_ESC
};

enum tty_mode
{
    TTY_RAW,
    TTY_COOKED
};

#endif // VIPEROS_TTY_ESCAPE
