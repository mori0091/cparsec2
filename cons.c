/* -*- coding:utf-8-unix -*- */

#include "cparsec2.h"

static StringResult run_cons_char(StringParser self, Source src) {
  Buffer str = buf_new();
  CharResult c = parse(self->head, src);
  if (c.error) {
    /* catch and re-throw exception */
    return (StringResult){.error = c.error};
  }
  buf_push(&str, c.result);
  StringResult s = parse(self->tail, src);
  if (s.error) {
    /* catch and re-throw exception */
    return s;
  }
  buf_append(&str, s.result);
  return (StringResult){.result = buf_finish(&str)};
}

StringParser cons_char(CharParser p, StringParser ps) {
  StringParser self = malloc(sizeof(struct stStringParser));
  self->run = run_cons_char;
  self->head = p;
  self->tail = ps;
  return self;
}
