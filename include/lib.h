#ifndef LIB_H
#define LIB_H

#include <libretro.h>
#include <stdint.h>

// Framebuffer dimensions
#define WIDTH 320
#define HEIGHT 240

void draw_string(int x, int y, const char *str, uint16_t color);
extern retro_log_printf_t log_cb;
extern int square_x;
extern int square_y;
extern retro_input_state_t input_state_cb;
extern char *script_path;  // Dynamic path to script.lua

#endif /* LIB_H */