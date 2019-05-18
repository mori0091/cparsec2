/* -*- coding:utf-8-unix -*- */

#include "cparsec2.h"

static StringResult run_many(StringParser p, Source src) {
  Buffer buf = buf_new();
  for (;;) {
    CharResult c = parse(p->parser, src);
    if (c.error) {
      /* catch and discard exception */
      free((void *)c.error);
      break;
    }
    buf_push(&buf, c.result);
  }
  buf_push(&buf, '\0');
  return (StringResult){.result = buf.data};
}

StringParser many(CharParser p) {
  StringParser ps = malloc(sizeof(struct stStringParser));
  ps->run = run_many;
  ps->parser = p;
  return ps;
}
