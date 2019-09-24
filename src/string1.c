/* -*- coding:utf-8-unix -*- */

#include "cparsec2.h"

#include <string.h>

static const char* to_s(const char* s) {
  return mem_printf("\"%s\"", s);
}

static const char* run_string1(void* arg, Source src, Ctx* ex) {
  const char* expected = (const char*)arg;
  Buff(Char) b = {0};
  Ctx ctx;
  TRY(&ctx) {
    const char* p = expected;
    for (; *p; p++) {
      buff_push(&b, parse(anyChar, src, &ctx));
    }
  }
  else {
    // too short input
    mem_free((void*)buff_finish(&b));
    cthrow(ex, ctx.msg);
  }
  const char* actual = buff_finish(&b);
  if (!strcmp(expected, actual)) {
    // exact match
    return actual;
  }
  ErrMsg m = {SysUnexpect, to_s(actual)};
  parseError(src, m);
  mem_free((void*)actual);
  cthrow(ex, error("unexpected tokens"));
}

PARSER(String) string1(const char* s) {
  return expects(to_s(s),
                 tryp(PARSER_GEN(String)(run_string1, (void*)s)));
}
