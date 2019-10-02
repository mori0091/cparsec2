/* -*- coding: utf-8-unix -*- */
#define TestSuite runParser
#include "testit.h"
#include <cparsec2.h>

test("cparsec2/anyChar with runParser() API") {
  cparsec2_init();
  ParseResult(Char) r = runParser(anyChar, "x");
  c_assert(r.succeeded);
  c_assert(r.consumed == 1);
  c_assert(r.ok = 'x');
  cparsec2_end();
}

test("cparsec2/number with runParser() API") {
  cparsec2_init();
  ParseResult(Int) r = runParser(number, "123");
  c_assert(r.succeeded);
  c_assert(r.consumed == 3);
  c_assert(r.ok = 123);
  cparsec2_end();
}
