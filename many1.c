/* -*- coding:utf-8-unix -*- */

#include "cparsec2.h"

static StringResult run_many1(StringParser self, Source src) {
  CharResult c = parse(self->parser, src);
  if (c.error) {
    /* catch and re-throw exception */
    return (StringResult){.error = c.error};
  }
  StringParser p = many(self->parser);
  StringResult s = parse(p, src);
  free(p);
  if (s.error) {
    /* catch and re-throw exception */
    return s;
  }
  Buffer str = buf_new();
  buf_push(&str, c.result);
  buf_append(&str, s.result);
  /* free((void*)s.result); */
  return (StringResult){.result = buf_finish(&str)};
}

StringParser many1(CharParser p) {
  StringParser self = malloc(sizeof(struct stStringParser));
  self->run = run_many1;
  self->parser = p;
  return self;
}
