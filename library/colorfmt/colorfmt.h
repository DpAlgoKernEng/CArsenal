#ifndef COLORFMT_H
#define COLORFMT_H

#include <stdarg.h>

enum {
  COLOR_PURPLE,
  COLOR_BLUE,
  COLOR_GREEN,
  COLOR_YELLOW,
  COLOR_RED,
  COLOR_PINK
};

#define print_purple(...) printcolor(COLOR_PURPLE, __VA_ARGS__)
#define print_blue(...) printcolor(COLOR_BLUE, __VA_ARGS__)
#define print_green(...) printcolor(COLOR_GREEN, __VA_ARGS__)
#define print_yellow(...) printcolor(COLOR_YELLOW, __VA_ARGS__)
#define print_red(...) printcolor(COLOR_RED, __VA_ARGS__)
#define print_pink(...) printcolor(COLOR_PINK, __VA_ARGS__)

void printcolor(int color_i, const char *fmt, ...);

#endif // DEBUG