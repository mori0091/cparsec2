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

CharParser char1(char c) {
  return genParser(run_char1, (void*)(intptr_t)c);
}
