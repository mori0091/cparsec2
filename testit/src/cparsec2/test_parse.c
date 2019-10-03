/* -*- coding: utf-8-unix -*- */
#define TestSuite parse
#include "testit.h"
#include <cparsec2.h>

test("cparsec2/anyChar") {
  cparsec2_init();
  Ctx ctx;
  TRY(&ctx) {
    Source src = Source_new("x");
    char r = parse(anyChar, src, &ctx);
    c_assert(eq(1, getSourceOffset(src)));
    c_assert(eq('x', r));
  }
  else {
    c_abort("must not fail");
  }
  cparsec2_end();
}

test("cparsec2/number") {
  cparsec2_init();
  Ctx ctx;
  TRY(&ctx) {
    Source src = Source_new("123");
    int r = parse(number, src, &ctx);
    c_assert(eq(3, getSourceOffset(src)));
    c_assert(eq(123, r));
  }
  else {
    c_abort("must not fail");
  }
  cparsec2_end();
}
