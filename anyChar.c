/* -*- coding:utf-8-unix -*- */

#include "cparsec2.h"

static char run_anyChar(Parser p, Source src) {
  UNUSED(p);
  char c = *src->p;
  if (!c) {
    return error("too short");
  }
  return c;
}

static struct stParser anyChar_ = {
                                   .run = run_anyChar,
};

Parser const anyChar = &anyChar_;
