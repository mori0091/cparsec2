/* -*- coding: utf-8-unix -*- */
/* ---- test suite 'simple' ---- */
#define TestSuite simple          /* test suite identifier */
#include "testit.h"

// test("desc" [, .key = value]...)
// -> same as testit(TestSuite, <a_unique_id>, "desc" [, .key = value]...)

test("the description must be a string literal") {
  c_assert(1 && "should success");
}

// No description is permitted (though not recommended)
test() {
  c_assert(1 && "should success");
}

// Empty code is permitted (will success)
test("an empty code test is permitted (will success)") {
}

test("a test with '.should_fail = true' will pass if assertion failed",
     .should_fail = true) {
  c_assert(0);
}
