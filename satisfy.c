/* -*- coding:utf-8-unix -*- */

#include "cparsec2.h"

static CharResult run_satisfy(CharParser p, Source src) {
  CharResult c = parse(anyChar, src);
  if (c.error) {
    return c; /* error */
  }
  if (!p->pred(c.result)) {
    return (CharResult){.error = error("not satisfy")};
  }
  return c;
}

CharParser satisfy(Predicate pred) {
  CharParser p = malloc(sizeof(struct stCharParser));
  p->run = run_satisfy;
  p->pred = pred;
  return p;
}
