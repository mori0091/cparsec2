/* -*- coding:utf-8-unix -*- */

#include "cparsec2.h"

static char run_noneOf(void* arg, Source src, Ctx* ex) {
  const char* unexpected = (const char*)arg;
  Ctx ctx;
  TRY(&ctx) {
    char c = peek(src, &ctx);
    for (const char* p = unexpected; *p; p++) {
      if (*p == c) {
        cthrow(&ctx, error("expects not one of \"%s\" but was '%c'",
                           unexpected, c));
      }
    }
    consume(src);
    return c;
  }
  else {
    cthrow(ex, ctx.msg);
  }
}

PARSER(Char) noneOf(const char* cs) {
  return PARSER_GEN(Char)(run_noneOf, (void*)cs);
}
