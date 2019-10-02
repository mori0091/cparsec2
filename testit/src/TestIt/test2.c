/* -*- coding: utf-8-unix -*- */
/* ---- test suite '2' ---- */
#define TestSuite 2
#include "testit.h"

static void pass(void) {
  c_assert(1);
}

test("a test can call other function which calls assertion macro") {
  pass();
}
