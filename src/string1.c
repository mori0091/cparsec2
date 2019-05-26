/* -*- coding:utf-8-unix -*- */

#include "cparsec2.h"

static const char* run_string1(void* arg, Source src, Ctx* ex) {
  const char* expected = (const char*)arg;
  Ctx ctx;
  TRY(&ctx) {
    const char* p = expected;
    while (*p) {
      parse(char1(*p++), src, &ctx);
    }
    return expected;
  }
  else {
    cthrow(ex, ctx.msg);
  }
}

StringParser string1(const char* s) {
  return genParser(run_string1, (void*)s);
}
