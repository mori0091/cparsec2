/* -*- coding: utf-8-unix -*- */
#define TestSuite chars
#include "testit.h"
#include <cparsec2.h>

struct expectation {
  bool assume;
  char expect;
  PARSER(Char) parser;
  const char* input;
};

static void should_pass_iff_(struct expectation x) {
  ParseResult(Char) result = runParser(x.parser, x.input);
  if (x.assume) {
    c_assert(result.succeeded);
    c_assert(result.consumed == 1);
    c_assert(result.ok == x.expect);
  } else {
    c_assert(!result.succeeded);
    c_assert(result.consumed == 0);
    c_assert(result.err.pos.line == 1);
    c_assert(result.err.pos.column == 1);
  }
}

#define should_pass_iff(...)                                             \
  should_pass_iff_((struct expectation){__VA_ARGS__})

/* a dataset generator (generates "\0", "\0x01", ..., "\0xff") */
static void* char_dataset(size_t i) {
  if (i < 256) {
    static char input[2] = {0};
    input[0] = (char)i;
    input[1] = '\0';
    return input;
  }
  return NULL;
}

/* test cases */

test("anyChar", .generator = char_dataset) {
  cparsec2_init();
  const char* input = testit_current_test_data();
  should_pass_iff(.parser = anyChar, .input = input,
                  .assume = (input[0] != 0), .expect = input[0]);
  cparsec2_end();
}

test("char1('a')", .generator = char_dataset) {
  cparsec2_init();
  const char* input = testit_current_test_data();
  should_pass_iff(.parser = char1('a'), .input = input,
                  .assume = (input[0] == 'a'), .expect = 'a');
  cparsec2_end();
}

test("digit", .generator = char_dataset) {
  cparsec2_init();
  const char* input = testit_current_test_data();
  should_pass_iff(.parser = digit, .input = input,
                  .assume = ('0' <= input[0] && input[0] <= '9'),
                  .expect = input[0]);
  cparsec2_end();
}

test("octDigit", .generator = char_dataset) {
  cparsec2_init();
  const char* input = testit_current_test_data();
  should_pass_iff(.parser = octDigit, .input = input,
                  .assume = ('0' <= input[0] && input[0] <= '7'),
                  .expect = input[0]);
  cparsec2_end();
}

test("hexDigit", .generator = char_dataset) {
  cparsec2_init();
  const char* input = testit_current_test_data();
  should_pass_iff(.parser = hexDigit, .input = input,
                  .assume = (('0' <= input[0] && input[0] <= '9') ||
                             ('a' <= input[0] && input[0] <= 'f') ||
                             ('A' <= input[0] && input[0] <= 'F')),
                  .expect = input[0]);
  cparsec2_end();
}

test("lower", .generator = char_dataset) {
  cparsec2_init();
  const char* input = testit_current_test_data();
  should_pass_iff(.parser = lower, .input = input,
                  .assume = ('a' <= input[0] && input[0] <= 'z'),
                  .expect = input[0]);
  cparsec2_end();
}

test("upper", .generator = char_dataset) {
  cparsec2_init();
  const char* input = testit_current_test_data();
  should_pass_iff(.parser = upper, .input = input,
                  .assume = ('A' <= input[0] && input[0] <= 'Z'),
                  .expect = input[0]);
  cparsec2_end();
}

test("alpha", .generator = char_dataset) {
  cparsec2_init();
  const char* input = testit_current_test_data();
  should_pass_iff(.parser = alpha, .input = input,
                  .assume = (('a' <= input[0] && input[0] <= 'z') ||
                             ('A' <= input[0] && input[0] <= 'Z')),
                  .expect = input[0]);
  cparsec2_end();
}

test("alnum", .generator = char_dataset) {
  cparsec2_init();
  const char* input = testit_current_test_data();
  should_pass_iff(.parser = alnum, .input = input,
                  .assume = (('a' <= input[0] && input[0] <= 'z') ||
                             ('A' <= input[0] && input[0] <= 'Z') ||
                             ('0' <= input[0] && input[0] <= '9')),
                  .expect = input[0]);
  cparsec2_end();
}

test("letter", .generator = char_dataset) {
  cparsec2_init();
  const char* input = testit_current_test_data();
  should_pass_iff(.parser = letter, .input = input,
                  .assume = (('a' <= input[0] && input[0] <= 'z') ||
                             ('A' <= input[0] && input[0] <= 'Z') ||
                             ('_' == input[0])),
                  .expect = input[0]);
  cparsec2_end();
}
