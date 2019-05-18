/* -*- coding:utf-8-unix -*- */

#include "cparsec2.h"

static CharResult run_anyChar(CharParser p, Source src) {
  UNUSED(p);
  CharResult c = peek(src);
  if (c.error) {
    return c; /* error */
  }
  consume(src);
  return c;
}

static struct stCharParser anyChar_ = {.run = run_anyChar};

CharParser const anyChar = &anyChar_;
