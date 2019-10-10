/* -*- coding:utf-8-unix -*- */

#include <cparsec2.h>

static char run_oneOf(void* arg, Source src, Ctx* ex) {
  const char* expected = (const char*)arg;
  Ctx ctx;
  TRY(&ctx) {
    char c = peek(src, &ctx);
    for (const char* p = expected; *p; p++) {
      if (*p == c) {
        consume(src);
        return c;
      }
    }
    cthrow(&ctx, error("expects one of \"%s\" but was '%c'", expected, c));
  }
  else {
    cthrow(ex, ctx.msg);
  }
}

PARSER(Char) oneOf(const char* cs) {
  return PARSER_GEN(Char)(run_oneOf, (void*)cs);
}
