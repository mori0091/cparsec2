/* -*- coding:utf-8-unix -*- */
/**
 * @file main.c
 * A sample application for self-testing of CPARSEC2 library.
 *
 * @note This `main.c` is not a part of the library itself.
 * @note i.e. `obj/main.o` will not be included in `lib/cparsec2.a`.
 */

#include "cparsec2.h"
#include <assert.h>

static void self_tests(void);

int main(void) {
  self_tests();
  return 0;
}

// -------------------------------------------------------------------

/* A user-defined parser function. */
const char* run_digit3(void* arg, Source src, Ctx* ex) {
  /* omit 'arg' since 'digit3' use no arguments */
  UNUSED(arg);

  Buff(Char) str = {0};
  Ctx ctx;
  TRY(&ctx) { /* try */
    for (int i = 0; i < 3; ++i) {
      buff_push(&str, parse(digit, src, &ctx));
    }
    return buff_finish(&str);
  }
  else { /* catch */
    mem_free(str.data);
    cthrow(ex, ctx.msg); /* re-throw */
  }
}

static void self_tests(void) {
  cparsec2_init(); /* must be called once at first */

  assert(PARSE_TEST(anyChar, "abc")); /* 'a' */
  assert(PARSE_TEST(anyChar, "bc"));  /* 'b' */
  assert(PARSE_TEST(anyChar, "123")); /* '1' */
  assert(!PARSE_TEST(anyChar, ""));   /* error */

  assert(PARSE_TEST(char1('a'), "abc"));  /* 'a' */
  assert(!PARSE_TEST(char1('b'), "abc")); /* error */
  assert(!PARSE_TEST(char1('a'), ""));    /* error */

  assert(PARSE_TEST(digit, "123"));  /* '1' */
  assert(PARSE_TEST(digit, "23"));   /* '2' */
  assert(!PARSE_TEST(digit, "abc")); /* error */
  assert(!PARSE_TEST(digit, ""));    /* error */

  assert(PARSE_TEST(lower, "abc"));  /* 'a' */
  assert(PARSE_TEST(lower, "bc"));   /* 'b' */
  assert(!PARSE_TEST(lower, "123")); /* error */
  assert(!PARSE_TEST(lower, "ABC")); /* error */
  assert(!PARSE_TEST(lower, ""));    /* error */

  assert(PARSE_TEST(upper, "ABC"));  /* 'A' */
  assert(PARSE_TEST(upper, "BC"));   /* 'B' */
  assert(!PARSE_TEST(upper, "123")); /* error */
  assert(!PARSE_TEST(upper, "abc")); /* error */
  assert(!PARSE_TEST(upper, ""));    /* error */

  assert(PARSE_TEST(alpha, "ABC"));  /* 'A' */
  assert(PARSE_TEST(alpha, "BC"));   /* 'B' */
  assert(!PARSE_TEST(alpha, "123")); /* error */
  assert(PARSE_TEST(alpha, "abc"));  /* 'a' */
  assert(!PARSE_TEST(alpha, ""));    /* error */

  assert(PARSE_TEST(alnum, "ABC")); /* 'A' */
  assert(PARSE_TEST(alnum, "BC"));  /* 'B' */
  assert(PARSE_TEST(alnum, "123")); /* '1' */
  assert(PARSE_TEST(alnum, "abc")); /* 'a' */
  assert(!PARSE_TEST(alnum, ""));   /* error */

  assert(PARSE_TEST(letter, "ABC"));  /* 'A' */
  assert(PARSE_TEST(letter, "BC"));   /* 'B' */
  assert(!PARSE_TEST(letter, "123")); /* error */
  assert(PARSE_TEST(letter, "abc"));  /* 'a' */
  assert(PARSE_TEST(letter, "_+*"));  /* '_' */
  assert(!PARSE_TEST(letter, "+*"));  /* error */
  assert(!PARSE_TEST(letter, ""));    /* error */

  assert(PARSE_TEST(spaces, "   a"));    /* () */
  assert(PARSE_TEST(spaces, " \t\n\r")); /* () */
  assert(PARSE_TEST(spaces, "a"));       /* () */
  assert(PARSE_TEST(spaces, ""));        /* () */

  assert(PARSE_TEST(many(anyChar), "12ab!*"));      /* "12ab!*" */
  assert(PARSE_TEST(many(digit), "123"));           /* "123" */
  assert(PARSE_TEST(many(digit), "12a"));           /* "12" */
  assert(PARSE_TEST(many(digit), "1234567890abc")); /* "1234567890" */
  assert(PARSE_TEST(many(digit), ""));              /* "" */
  assert(PARSE_TEST(many(digit), "abc"));           /* "" */

  assert(PARSE_TEST(many1(anyChar), "12ab!*"));      /* "12ab!*" */
  assert(PARSE_TEST(many1(digit), "123"));           /* "123" */
  assert(PARSE_TEST(many1(digit), "12a"));           /* "12" */
  assert(PARSE_TEST(many1(digit), "1234567890abc")); /* "1234567890" */
  assert(!PARSE_TEST(many1(digit), ""));             /* error */
  assert(!PARSE_TEST(many1(digit), "abc"));          /* error */

  assert(PARSE_TEST(seq(letter, digit, digit), "a12"));  /* "a12" */
  assert(PARSE_TEST(seq(letter, digit, digit), "_123")); /* "_12" */
  assert(!PARSE_TEST(seq(letter, digit, digit), "123")); /* error */

  assert(PARSE_TEST(cons(letter, many(digit)), "a1234"));  /* "a1234" */
  assert(PARSE_TEST(cons(letter, many(digit)), "abc123")); /* "a" */
  assert(PARSE_TEST(cons(letter, many(digit)), "a123bc")); /* "a123" */

  assert(PARSE_TEST(string1("a1234"), "a1234"));   /* "a1234" */
  assert(PARSE_TEST(string1("a123"), "a1234"));    /* "a123" */
  assert(PARSE_TEST(string1("a123"), "a123bc"));   /* "a123" */
  assert(!PARSE_TEST(string1("a1234"), "a123"));   /* error */
  assert(!PARSE_TEST(string1("a1234"), "a123bc")); /* error */

  PARSER(String) digit3 = PARSER_GEN(String)(run_digit3, NULL);
  assert(PARSE_TEST(digit3, "1234"));  /* "123" */
  assert(PARSE_TEST(digit3, "123"));   /* "123" */
  assert(!PARSE_TEST(digit3, "12"));   /* error */
  assert(!PARSE_TEST(digit3, "a123")); /* error */

  assert(PARSE_TEST(cons(upper, digit3), "A123"));  /* "A123" */
  assert(!PARSE_TEST(cons(upper, digit3), "a123")); /* error */

  assert(PARSE_TEST(number, "1234"));  /* 1234 */
  assert(PARSE_TEST(number, "123"));   /* 123 */
  assert(PARSE_TEST(number, "12"));    /* 12 */
  assert(PARSE_TEST(number, "1"));     /* 1 */
  assert(!PARSE_TEST(number, ""));     /* error */
  assert(!PARSE_TEST(number, "a123")); /* error */

  PARSER(Char) token_plus = token((char)'+');
  assert(!PARSE_TEST(token_plus, "1 + 2")); /* error */
  assert(PARSE_TEST(token_plus, "+ 2"));    /* '+' */
  assert(!PARSE_TEST(token_plus, "2"));     /* error */

  assert(PARSE_TEST(either("ab", "bc"), "ab"));  /* "ab" */
  assert(PARSE_TEST(either("ab", "bc"), "bc"));  /* "bc" */
  assert(!PARSE_TEST(either("ab", "bc"), "ac")); /* error */

  assert(PARSE_TEST(either("ab", "ac"), "ac")); /* "ac" */

  PARSER(Char) a = char1('a');
  PARSER(Char) b = char1('b');
  PARSER(Char) c = char1('c');
  assert(PARSE_TEST(either(seq(a, b), seq(a, c)), "ab"));  /* "ab" */
  assert(!PARSE_TEST(either(seq(a, b), seq(a, c)), "ac")); /* error */
  assert(PARSE_TEST(either(tryp(seq(a, b)), seq(a, c)), "ac")); /* "ac" */

  assert(PARSE_TEST(either("ab", "bc"), "ab"));  /* "ab" */
  assert(PARSE_TEST(either("ab", "bc"), "bc"));  /* "bc" */
  assert(!PARSE_TEST(either("ab", "bc"), "ac")); /* error */

  assert(!PARSE_TEST(either("ab", "cd"), "xy")); /* error */

  assert(PARSE_TEST(FOLDL(either, "ab", "cd", "ef"), "ab"));  /* "ab" */
  assert(PARSE_TEST(FOLDL(either, "ab", "cd", "ef"), "cd"));  /* "cd" */
  assert(PARSE_TEST(FOLDL(either, "ab", "cd", "ef"), "ef"));  /* "ef" */
  assert(!PARSE_TEST(FOLDL(either, "ab", "cd", "ef"), "xy")); /* error */

  assert(PARSE_TEST(skip((char)'a'), "ab"));         /* () */
  assert(PARSE_TEST(many(skip((char)'a')), "aabb")); /* [(), ()] */
  assert(PARSE_TEST(many(skip(either((char)'a', (char)'b'))), "aabb"));
  /* -> [(), (), (), ()]  */

  assert(PARSE_TEST(many(number), "123 456 789"));
  /* -> [123, 456, 789] */

  assert(PARSE_TEST(many(number), ""));
  /* -> [] */

  assert(PARSE_TEST(many(token(many1(digit))), "123 456 789"));
  /* -> ["123", "456", "789"] */

  assert(PARSE_TEST(many(token(many1(digit))), ""));
  /* -> [] */

  assert(PARSE_TEST(many(skip1st(char1(','), number)), ",123,456,789"));
  /* -> [123, 456, 789] */

  assert(PARSE_TEST(many(skip1st(char1(','), many1(digit))),
                    ",123,456,789"));
  /* -> ["123", "456", "789"] */

  assert(!PARSE_TEST(many(endOfFile), ""));       /* error */
  assert(!PARSE_TEST(many(spaces), "aaa"));       /* error */
  assert(!PARSE_TEST(many(skip(endOfFile)), "")); /* error */
  assert(!PARSE_TEST(many(skip(spaces)), "aaa")); /* error */

  assert(!PARSE_TEST(many1(endOfFile), ""));       /* error */
  assert(!PARSE_TEST(many1(spaces), "aaa"));       /* error */
  assert(!PARSE_TEST(many1(skip(endOfFile)), "")); /* error */
  assert(!PARSE_TEST(many1(skip(spaces)), "aaa")); /* error */

  cparsec2_end();
}
