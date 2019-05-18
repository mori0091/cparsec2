/* -*- coding:utf-8-unix -*- */

#include "cparsec2.h"

static char run_satisfy(CharParser p, Source src) {
  char c = parse(anyChar, src);
  if (!c) {
    return c; /* error */
  }
  if (!p->pred(c)) {
    return error("not satisfy");
  }
  return c;
}

CharParser satisfy(Predicate pred) {
  CharParser p = malloc(sizeof(struct stCharParser));
  p->run = run_satisfy;
  p->pred = pred;
  return p;
}
