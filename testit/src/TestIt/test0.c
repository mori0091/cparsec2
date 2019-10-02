/* -*- coding: utf-8-unix -*- */
#include "testit.h"

// testit(suite, name, "desc" [, .key = value]...)
// - suite : test suite identifier (no identifier is permitted)
// -  name : testcase identifier
// -  desc : short description of the testcase

/* ---- test suite 'simple' ---- */
testit(simple, A, "A test named 'simple::A'") {
  c_assert(1);
}

/* ---- test suite '2' ---- */
testit(2, A, "A test named '2::A'") {
  c_assert(1);
}

/* ---- test suite '' ---- */
testit(, A, "A test named '::A'") {
  c_assert(1);
}
