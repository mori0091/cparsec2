/* -*- coding:utf-8-unix -*- */

#include "cparsec2.h"

const char *error(const char *fmt, ...) {
  char buf[1024] = {0};
  va_list ap;
  va_start(ap, fmt);
  vsprintf(buf, fmt, ap);
  return strdup(buf);
}

void show_char(char c) { printf("'%c'\n", c); }
