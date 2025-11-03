#ifndef VIPEROS_TTY_SCREEN
#define VIPEROS_TTY_SCREEN 1

void screen_init(void);

typedef unsigned long size_t;
typedef unsigned char uint8_t;
typedef unsigned int uint32_t;

size_t get_horiz(void);
size_t get_vert(void);
size_t get_pitch(void);

void clear_screen(void);
void flush(void);

void plot_char(unsigned char c, int x, int y, uint32_t fg, uint32_t bg);

#endif // VIPEROS_TTY_SCREEN
