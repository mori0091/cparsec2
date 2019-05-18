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

char parse(CharParser p, Source src) { return p->run(p, src); }

void parseTest(CharParser p, const char *input) {
  struct stSource src;
  src.input = input;
  src.p = input;
  char c = parse(p, &src);
  if (c) {
    printf("'%c'\n", c);
  }
}
