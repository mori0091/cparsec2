/* -*- coding:utf-8-unix -*- */

#include "cparsec2.h"

static char run_char1(Parser p, Source src) {
  char c = parse(anyChar, src);
  if (!c) {
    return c; /* error */
  }
  if (p->expected != c) {
    return error("expects '%c' but was '%c'", p->expected, c);
  }
  return c;
}

Parser char1(char c) {
  Parser p = malloc(sizeof(struct stParser));
  p->run = run_char1;
  p->expected = c;
  return p;
}
