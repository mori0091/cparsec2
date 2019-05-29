/* -*- coding:utf-8-unix -*- */

#include "cparsec2.h"

static char run_char1(void* arg, Source src, Ctx* ex) {
  char expected = (char)(intptr_t)arg;
  Ctx ctx;
  TRY(&ctx) {
    char c = peek(src, &ctx);
    if (expected == c) {
      consume(src);
      return c;
    }
    cthrow(&ctx, error("expects '%c' but was '%c'", expected, c));
  }
  else {
    cthrow(ex, ctx.msg);
  }
}

PARSER(Char) char1(char c) {
  return PARSER_GEN(Char)(run_char1, (void*)(intptr_t)c);
}
