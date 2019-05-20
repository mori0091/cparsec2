/* -*- coding:utf-8-unix -*- */

#include "cparsec2.h"

static CharResult run_char1(void *arg, Source src) {
  CharResult c = peek(src);
  if (c.error) {
    return c; /* error */
  }
  char expected = (char)(intptr_t)arg;
  if (expected != c.result) {
    return (CharResult){.error = error("expects '%c' but was '%c'", expected, c.result)};
  }
  consume(src);
  return c;
}

CharParser char1(char c) {
  return genParser(run_char1, (void *)(intptr_t)c);
}
