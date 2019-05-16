/* -*- coding:utf-8-unix -*- */

#include "cparsec2.h"

static char run_satisfy(Parser p, Source src) {
  char c = parse(anyChar, src);
  if (!c) {
    return c; /* error */
  }
  if (!p->pred(c)) {
    return error("not satisfy");
  }
  return c;
}

Parser satisfy(Predicate pred) {
  Parser p = malloc(sizeof(struct stParser));
  p->run = run_satisfy;
  p->pred = pred;
  return p;
}
