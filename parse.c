/* -*- coding:utf-8-unix -*- */

#include "cparsec2.h"

char error(const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  printf("error:");
  vprintf(fmt, ap);
  printf("\n");
  return '\0';
}

void show(char c);

void parseTest(CharParser p, const char *input) {
  struct stSource src = {.input = input, .p = input};
  show(parse(p, &src));
}

void show(char c) {
  if (c) {
    printf("'%c'\n", c);
  }
}
