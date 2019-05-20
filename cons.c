/* -*- coding:utf-8-unix -*- */

#include "cparsec2.h"

struct cons_arg {
  CharParser head;
  StringParser tail;
};

static StringResult run_cons_char(void *arg, Source src) {
  struct cons_arg *self = (struct cons_arg *)arg;
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
  struct cons_arg *arg = malloc(sizeof(struct cons_arg));
  arg->head = p;
  arg->tail = ps;
  return genParser(run_cons_char, arg);
}
