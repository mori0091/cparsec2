/* -*- coding: utf-8-unix -*- */

/* parmeterized test sample */

#define TestSuite parametric
#include "testit.h"

static int mul(int x, int y) {
  return x * y;
}

struct data {
  int lhs;
  int rhs;
  int expect;
};

#define array_sizeof(a) (sizeof(a) / sizeof(a[0]))

// A generator function, which generates a test parameter one by one.
//
// Generator function takes one argument (index or iteration count),
// and returns corresponding value as void* pointer.
//
// If there is no more value, generator must return NULL.
// (inifinite generator is not permitted)
static void* dataset1(size_t i) {
  static struct data data[] = {
      {1, 2, 2},  // (#0) 1 * 2 == 2
      {2, 3, 6},  // (#1) 2 * 3 == 6
      {3, 4, 12}, // (#2) 3 * 4 == 12
      {4, 5, 20}, // (#3) 4 * 5 == 20
  };
  if (i < array_sizeof(data)) {
    return &data[i];
  }
  return NULL;
}

// if .generator was given, the test is executed iteratively for each
// value provided by generator function until the generator returns NULL.
test("mul(x,y) with dataset1", .generator = dataset1) {
  // get the current value provided by generator function.
  struct data* x = testit_current_test_data();
  c_assert(eq(x->expect, mul(x->lhs, x->rhs)));
}

// if no .generator was given, testit_current_test_data() API fails
test("mul(x,y) without dataset should fail", .should_fail = true) {
  struct data* x = testit_current_test_data();
  (void)x; // omit warning unused variable (-Wunused-variable)
}
