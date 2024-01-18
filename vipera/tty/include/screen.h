#ifndef VIPEROS_VIPERA_TTY_SCREEN_H
#define VIPEROS_VIPERA_TTY_SCREEN_H 1

void screen_init();

typedef unsigned long size_t;
typedef unsigned char uint8_t;
typedef unsigned int uint32_t;

size_t get_horiz();
size_t get_vert();
size_t get_pitch();

void clear_screen();
void flush();

void plot_char(unsigned char c, int x, int y, uint32_t fg, uint32_t bg);

#endif // VIPEROS_VIPERA_TTY_SCREEN_H