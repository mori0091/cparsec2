/* -*- coding:utf-8-unix -*- */

#include "cparsec2.h"

int main(int argc, char **argv) {
  UNUSED(argc);
  UNUSED(argv);

  cparsec2_init(); /* must be called once at first */

  PARSE_TEST(anyChar, "abc"); /* a */
  PARSE_TEST(anyChar, "bc");  /* b */
  PARSE_TEST(anyChar, "123"); /* 1 */
  PARSE_TEST(anyChar, "");    /* "error:too short" */

  PARSE_TEST(char1('a'), "abc"); /* a */
  PARSE_TEST(char1('b'), "abc"); /* error:expects 'b' but was 'a' */
  PARSE_TEST(char1('a'), "");    /* "error:too short" */

  PARSE_TEST(digit, "123"); /* 1 */
  PARSE_TEST(digit, "23");  /* 2 */
  PARSE_TEST(digit, "abc"); /* "error:not satisfy" */
  PARSE_TEST(digit, "");    /* "error:too short" */

  PARSE_TEST(lower, "abc"); /* a */
  PARSE_TEST(lower, "bc");  /* b */
  PARSE_TEST(lower, "123"); /* "error:not satisfy" */
  PARSE_TEST(lower, "ABC"); /* "error:not satisfy" */
  PARSE_TEST(lower, "");    /* "error:too short" */

  PARSE_TEST(upper, "ABC"); /* A */
  PARSE_TEST(upper, "BC");  /* B */
  PARSE_TEST(upper, "123"); /* "error:not satisfy" */
  PARSE_TEST(upper, "abc"); /* "error:not satisfy" */
  PARSE_TEST(upper, "");    /* "error:too short" */

  PARSE_TEST(alpha, "ABC"); /* A */
  PARSE_TEST(alpha, "BC");  /* B */
  PARSE_TEST(alpha, "123"); /* "error:not satisfy" */
  PARSE_TEST(alpha, "abc"); /* a */
  PARSE_TEST(alpha, "");    /* "error:too short" */

  PARSE_TEST(alnum, "ABC"); /* A */
  PARSE_TEST(alnum, "BC");  /* B */
  PARSE_TEST(alnum, "123"); /* 1 */
  PARSE_TEST(alnum, "abc"); /* a */
  PARSE_TEST(alnum, "");    /* "error:too short" */

  PARSE_TEST(letter, "ABC"); /* A */
  PARSE_TEST(letter, "BC");  /* B */
  PARSE_TEST(letter, "123"); /* "error:not satisfy" */
  PARSE_TEST(letter, "abc"); /* a */
  PARSE_TEST(letter, "_+*"); /* _ */
  PARSE_TEST(letter, "+*");  /* "error:not satisfy" */
  PARSE_TEST(letter, "");    /* "error:too short" */

  PARSE_TEST(many(anyChar), "12ab!*");      /* "12ab!*" */
  PARSE_TEST(many(digit), "123");           /* "123" */
  PARSE_TEST(many(digit), "12a");           /* "12" */
  PARSE_TEST(many(digit), "1234567890abc"); /* "1234567890" */
  PARSE_TEST(many(digit), "");              /* "" */
  PARSE_TEST(many(digit), "abc");           /* "" */

  PARSE_TEST(many1(anyChar), "12ab!*");      /* "12ab!*" */
  PARSE_TEST(many1(digit), "123");           /* "123" */
  PARSE_TEST(many1(digit), "12a");           /* "12" */
  PARSE_TEST(many1(digit), "1234567890abc"); /* "1234567890" */
  PARSE_TEST(many1(digit), "");              /* "error:too short" */
  PARSE_TEST(many1(digit), "abc");           /* "error:not satisfy" */

  PARSE_TEST(seq(letter, digit, digit), "a12");  /* "a12" */
  PARSE_TEST(seq(letter, digit, digit), "_123"); /* "_12" */
  PARSE_TEST(seq(letter, digit, digit), "123");  /* "error:not satisfy" */

  /* cannot compile [-Wincompatible-pointer-types] */
  // PARSE_TEST(seq(letter, many(digit)), "a1234"); /* "a1234" */
  return 0;
}
