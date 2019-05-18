/* -*- coding:utf-8-unix -*- */

#include "cparsec2.h"

static StringResult run_many(StringParser p, Source src) {
  char * buf = malloc(8);
  int capacity = 8;
  int len = 0;
  for (;;) {
    CharResult c = parse(p->parser, src);
    if (c.error) break;
    if (len == capacity - 1) {
      capacity *= 2;
      buf = realloc(buf, capacity);
    }
    buf[len++] = c.result;
  }
  buf[len] = '\0';
  return (StringResult){.result = buf};
}

StringParser many(CharParser p) {
  StringParser ps = malloc(sizeof(struct stStringParser));
  ps->run = run_many;
  ps->parser = p;
  return ps;
}
