#include "colorfmt.h"
#include <stdio.h>

static const char *colors[] = {"\x1b[94m", "\x1b[36m", "\x1b[32m",
                               "\x1b[33m", "\x1b[31m", "\x1b[35m"};

void printcolor(int color_i, const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  fprintf(stderr, "%s", colors[color_i]);
  vfprintf(stderr, fmt, ap);
  fprintf(stderr, "%s", "\x1b[0m");
  fflush(stderr);
  va_end(ap);
}