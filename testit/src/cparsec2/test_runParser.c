/* -*- coding: utf-8-unix -*- */
#define TestSuite runParser
#include "testit.h"
#include <cparsec2.h>

test("cparsec2/anyChar with runParser() API") {
  cparsec2_init();
  ParseResult(Char) r = runParser(anyChar, "x");
  c_assert(r.succeeded);
  c_assert(eq(1, r.consumed));
  c_assert(eq('x', r.ok));
  cparsec2_end();
}

test("cparsec2/number with runParser() API") {
  cparsec2_init();
  ParseResult(Int) r = runParser(number, "123");
  c_assert(r.succeeded);
  c_assert(eq(3, r.consumed));
  c_assert(eq(123, r.ok));
  cparsec2_end();
}
