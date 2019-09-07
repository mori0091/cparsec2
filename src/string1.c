/* -*- coding:utf-8-unix -*- */

#include "cparsec2.h"

static const char* run_string1(void* arg, Source src, Ctx* ex) {
  const char* expected = (const char*)arg;
  Ctx ctx;
  TRY(&ctx) {
    const char* p = expected;
    while (*p) {
      parse(char1(*p), src, &ctx);
      p++;
    }
    return expected;
  }
  else {
    ErrMsg m = {Expect, mem_printf("\"%s\"", expected)};
    parseError(src, m);
    cthrow(ex, ctx.msg);
  }
}

PARSER(String) string1(const char* s) {
  return PARSER_GEN(String)(run_string1, (void*)s);
}
