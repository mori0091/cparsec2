/* -*- coding:utf-8-unix -*- */

#include "cparsec2.h"

static StringResult run_many(StringParser self, Source src) {
  Buffer str = buf_new();
  for (;;) {
    CharResult c = parse(self->parser, src);
    if (c.error) {
      /* catch and discard exception */
      free((void *)c.error);
      break;
    }
    buf_push(&str, c.result);
  }
  return (StringResult){.result = buf_finish(&str)};
}

StringParser many(CharParser p) {
  StringParser self = malloc(sizeof(struct stStringParser));
  self->run = run_many;
  self->parser = p;
  return self;
}
