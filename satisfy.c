/* -*- coding:utf-8-unix -*- */

#include "cparsec2.h"

struct satisfy_arg {
  Predicate pred;
};

static CharResult run_satisfy(void *arg, Source src) {
  CharResult c = peek(src);
  if (c.error) {
    return c; /* error */
  }
  struct satisfy_arg *self = (struct satisfy_arg *)arg;
  if (!self->pred(c.result)) {
    return (CharResult){.error = error("not satisfy")};
  }
  consume(src);
  return c;
}

CharParser satisfy(Predicate pred) {
  struct satisfy_arg *arg = malloc(sizeof(struct satisfy_arg));
  arg -> pred = pred;
  return genParser(run_satisfy, arg);
}
