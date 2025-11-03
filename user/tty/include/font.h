#ifndef VIPEROS_TTY_FONT
#define VIPEROS_TTY_FONT 1

typedef unsigned long size_t;

void font_init(void);
void read_font(unsigned char c, char data[8]);

#endif // VIPEROS_TTY_FONT
