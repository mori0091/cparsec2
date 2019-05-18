/* -*- coding:utf-8-unix -*- */

#include "cparsec2.h"

static char run_char1(CharParser p, Source src) {
  char c = parse(anyChar, src);
  if (!c) {
    return c; /* error */
  }
  if (p->expected != c) {
    return error("expects '%c' but was '%c'", p->expected, c);
  }
  return c;
}

CharParser char1(char c) {
  CharParser p = malloc(sizeof(struct stCharParser));
  p->run = run_char1;
  p->expected = c;
  return p;
}
