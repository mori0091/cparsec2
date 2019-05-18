/* -*- coding:utf-8-unix -*- */

#include "cparsec2.h"

int main(int argc, char **argv) {
  UNUSED(argc);
  UNUSED(argv);

  PARSE_TEST(anyChar, "abc"); /* a */
  PARSE_TEST(anyChar, "bc");  /* b */
  PARSE_TEST(anyChar, "123"); /* 1 */
  PARSE_TEST(anyChar, "");    /* "error:too short" */

  PARSE_TEST(char1('a'), "abc"); /* a */
  PARSE_TEST(char1('b'), "abc"); /* error:expects 'b' but was 'a' */
  PARSE_TEST(char1('a'), "");    /* "error:too short" */

  CharParser digit = satisfy(is_digit);
  PARSE_TEST(digit, "123"); /* 1 */
  PARSE_TEST(digit, "23");  /* 2 */
  PARSE_TEST(digit, "abc"); /* "error:not satisfy" */
  PARSE_TEST(digit, "");    /* "error:too short" */

  CharParser lower = satisfy(is_lower);
  PARSE_TEST(lower, "abc"); /* a */
  PARSE_TEST(lower, "bc");  /* b */
  PARSE_TEST(lower, "123"); /* "error:not satisfy" */
  PARSE_TEST(lower, "ABC"); /* "error:not satisfy" */
  PARSE_TEST(lower, "");    /* "error:too short" */

  CharParser upper = satisfy(is_upper);
  PARSE_TEST(upper, "ABC"); /* A */
  PARSE_TEST(upper, "BC");  /* B */
  PARSE_TEST(upper, "123"); /* "error:not satisfy" */
  PARSE_TEST(upper, "abc"); /* "error:not satisfy" */
  PARSE_TEST(upper, "");    /* "error:too short" */

  CharParser alpha = satisfy(is_alpha);
  PARSE_TEST(alpha, "ABC"); /* A */
  PARSE_TEST(alpha, "BC");  /* B */
  PARSE_TEST(alpha, "123"); /* "error:not satisfy" */
  PARSE_TEST(alpha, "abc"); /* a */
  PARSE_TEST(alpha, "");    /* "error:too short" */

  CharParser alnum = satisfy(is_alnum);
  PARSE_TEST(alnum, "ABC"); /* A */
  PARSE_TEST(alnum, "BC");  /* B */
  PARSE_TEST(alnum, "123"); /* 1 */
  PARSE_TEST(alnum, "abc"); /* a */
  PARSE_TEST(alnum, "");    /* "error:too short" */

  CharParser letter = satisfy(is_letter);
  PARSE_TEST(letter, "ABC"); /* A */
  PARSE_TEST(letter, "BC");  /* B */
  PARSE_TEST(letter, "123"); /* "error:not satisfy" */
  PARSE_TEST(letter, "abc"); /* a */
  PARSE_TEST(letter, "_+*"); /* _ */
  PARSE_TEST(letter, "+*");  /* "error:not satisfy" */
  PARSE_TEST(letter, "");    /* "error:too short" */

  PARSE_TEST(many(anyChar), "12ab!*"); /* "12ab!*" */
  PARSE_TEST(many(digit), "123"); /* "123" */
  PARSE_TEST(many(digit), "12a"); /* "12" */
  PARSE_TEST(many(digit), "1234567890abc"); /* "1234567890" */
  PARSE_TEST(many(digit), "");    /* "" */

  return 0;
}
