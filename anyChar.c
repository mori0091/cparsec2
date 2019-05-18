/* -*- coding:utf-8-unix -*- */

#include "cparsec2.h"

static CharResult run_anyChar(CharParser p, Source src) {
  UNUSED(p);
  char c = *src->p;
  if (!c) {
    return (CharResult){.error = error("too short")};
  }
  return (CharResult){.result = c};
}

static struct stCharParser anyChar_ = {.run = run_anyChar};

CharParser const anyChar = &anyChar_;
