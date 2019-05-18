/* -*- coding:utf-8-unix -*- */

#include "cparsec2.h"

static CharResult run_char1(CharParser p, Source src) {
  CharResult c = parse(anyChar, src);
  if (c.error) {
    return c; /* error */
  }
  if (p->expected != c.result) {
    return (CharResult){.error = error("expects '%c' but was '%c'", p->expected, c.result)};
  }
  return c;
}

CharParser char1(char c) {
  CharParser p = malloc(sizeof(struct stCharParser));
  p->run = run_char1;
  p->expected = c;
  return p;
}
