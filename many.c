/* -*- coding:utf-8-unix -*- */

#include "cparsec2.h"

static StringResult run_many(StringParser self, Source src) {
  Buffer buf = buf_new();
  for (;;) {
    CharResult c = parse(self->parser, src);
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
  StringParser self = malloc(sizeof(struct stStringParser));
  self->run = run_many;
  self->parser = p;
  return self;
}
