/* -*- coding:utf-8-unix -*- */
/**
 * @file main.c
 * A sample application for self-testing of CPARSEC2 library.
 *
 * @note This `main.c` is not a part of the library itself.
 * @note i.e. `obj/main.o` will not be included in `lib/cparsec2.a`.
 */

#include <cparsec2.h>

#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

static void self_tests(void);
static void printBoldGreen(const char* fmt, ...);

int main(void) {
  self_tests();
  printBoldGreen("All tests passed\n");
  return 0;
}

// -------------------------------------------------------------------

#ifdef _WIN32
/* for Windows platform (including Mingw) */
static bool is_ANSI_escape_available(void) {
  return false;
}
#else
/* for Other platform (including MSYS) */
static bool is_ANSI_escape_available(void) {
  return isatty(fileno(stdout));
}
#endif

static void printBoldGreen(const char* fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  if (!is_ANSI_escape_available()) {
    vprintf(fmt, ap);
  } else {
    printf("\033[1;32m");
    vprintf(fmt, ap);
    printf("\033[0m");
  }
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

  // assert(PARSE_TEST(anyChar, "abc")); /* 'a' */
  {
    ParseResult(Char) r = runParser(anyChar, "abc");
    assert(r.consumed == 1);
    assert(r.succeeded);
    assert(r.ok == 'a');
  }

  // assert(PARSE_TEST(anyChar, "bc"));  /* 'b' */
  {
    ParseResult(Char) r = runParser(anyChar, "bc");
    assert(r.consumed == 1);
    assert(r.succeeded);
    assert(r.ok == 'b');
  }

  // assert(PARSE_TEST(anyChar, "123")); /* '1' */
  {
    ParseResult(Char) r = runParser(anyChar, "123");
    assert(r.consumed == 1);
    assert(r.succeeded);
    assert(r.ok == '1');
  }

  // assert(!PARSE_TEST(anyChar, ""));   /* error */
  {
    ParseResult(Char) r = runParser(anyChar, "");
    assert(r.consumed == 0);
    assert(!r.succeeded);
    assert(r.err.pos.line == 1);
    assert(r.err.pos.column == 1);
  }

  // assert(PARSE_TEST(char1('a'), "abc"));  /* 'a' */
  {
    ParseResult(Char) r = runParser(char1('a'), "abc");
    assert(r.consumed == 1);
    assert(r.succeeded);
    assert(r.ok == 'a');
  }

  // assert(!PARSE_TEST(char1('b'), "abc")); /* error */
  {
    ParseResult(Char) r = runParser(char1('b'), "abc");
    assert(r.consumed == 0);
    assert(!r.succeeded);
    assert(r.err.pos.line == 1);
    assert(r.err.pos.column == 1);
  }

  // assert(!PARSE_TEST(char1('a'), ""));    /* error */
  {
    ParseResult(Char) r = runParser(char1('a'), "");
    assert(r.consumed == 0);
    assert(!r.succeeded);
    assert(r.err.pos.line == 1);
    assert(r.err.pos.column == 1);
  }

  // assert(PARSE_TEST(digit, "123"));  /* '1' */
  {
    ParseResult(Char) r = runParser(digit, "123");
    assert(r.consumed == 1);
    assert(r.succeeded);
    assert(r.ok == '1');
  }

  // assert(PARSE_TEST(digit, "23"));   /* '2' */
  {
    ParseResult(Char) r = runParser(digit, "23");
    assert(r.consumed == 1);
    assert(r.succeeded);
    assert(r.ok == '2');
  }

  // assert(!PARSE_TEST(digit, "abc")); /* error */
  {
    ParseResult(Char) r = runParser(digit, "abc");
    assert(r.consumed == 0);
    assert(!r.succeeded);
    assert(r.err.pos.line == 1);
    assert(r.err.pos.column == 1);
  }

  // assert(!PARSE_TEST(digit, ""));    /* error */
  {
    ParseResult(Char) r = runParser(digit, "");
    assert(r.consumed == 0);
    assert(!r.succeeded);
    assert(r.err.pos.line == 1);
    assert(r.err.pos.column == 1);
  }

  // assert(PARSE_TEST(lower, "abc"));  /* 'a' */
  {
    ParseResult(Char) r = runParser(lower, "abc");
    assert(r.consumed == 1);
    assert(r.succeeded);
    assert(r.ok == 'a');
  }

  // assert(PARSE_TEST(lower, "bc"));   /* 'b' */
  {
    ParseResult(Char) r = runParser(lower, "bc");
    assert(r.consumed == 1);
    assert(r.succeeded);
    assert(r.ok == 'b');
  }

  // assert(!PARSE_TEST(lower, "123")); /* error */
  {
    ParseResult(Char) r = runParser(lower, "123");
    assert(r.consumed == 0);
    assert(!r.succeeded);
    assert(r.err.pos.line == 1);
    assert(r.err.pos.column == 1);
  }

  // assert(!PARSE_TEST(lower, "ABC")); /* error */
  {
    ParseResult(Char) r = runParser(lower, "ABC");
    assert(r.consumed == 0);
    assert(!r.succeeded);
    assert(r.err.pos.line == 1);
    assert(r.err.pos.column == 1);
  }

  // assert(!PARSE_TEST(lower, ""));    /* error */
  {
    ParseResult(Char) r = runParser(lower, "");
    assert(r.consumed == 0);
    assert(!r.succeeded);
    assert(r.err.pos.line == 1);
    assert(r.err.pos.column == 1);
  }

  // assert(PARSE_TEST(upper, "ABC"));  /* 'A' */
  {
    ParseResult(Char) r = runParser(upper, "ABC");
    assert(r.consumed == 1);
    assert(r.succeeded);
    assert(r.ok == 'A');
  }

  // assert(PARSE_TEST(upper, "BC"));   /* 'B' */
  {
    ParseResult(Char) r = runParser(upper, "BC");
    assert(r.consumed == 1);
    assert(r.succeeded);
    assert(r.ok == 'B');
  }

  // assert(!PARSE_TEST(upper, "123")); /* error */
  {
    ParseResult(Char) r = runParser(upper, "123");
    assert(r.consumed == 0);
    assert(!r.succeeded);
    assert(r.err.pos.line == 1);
    assert(r.err.pos.column == 1);
  }

  // assert(!PARSE_TEST(upper, "abc")); /* error */
  {
    ParseResult(Char) r = runParser(upper, "abc");
    assert(r.consumed == 0);
    assert(!r.succeeded);
    assert(r.err.pos.line == 1);
    assert(r.err.pos.column == 1);
  }

  // assert(!PARSE_TEST(upper, ""));    /* error */
  {
    ParseResult(Char) r = runParser(upper, "");
    assert(r.consumed == 0);
    assert(!r.succeeded);
    assert(r.err.pos.line == 1);
    assert(r.err.pos.column == 1);
  }

  // assert(PARSE_TEST(alpha, "ABC"));  /* 'A' */
  {
    ParseResult(Char) r = runParser(alpha, "ABC");
    assert(r.consumed == 1);
    assert(r.succeeded);
    assert(r.ok == 'A');
  }

  // assert(PARSE_TEST(alpha, "BC"));   /* 'B' */
  {
    ParseResult(Char) r = runParser(alpha, "BC");
    assert(r.consumed == 1);
    assert(r.succeeded);
    assert(r.ok == 'B');
  }

  // assert(!PARSE_TEST(alpha, "123")); /* error */
  {
    ParseResult(Char) r = runParser(alpha, "123");
    assert(r.consumed == 0);
    assert(!r.succeeded);
    assert(r.err.pos.line == 1);
    assert(r.err.pos.column == 1);
  }

  // assert(PARSE_TEST(alpha, "abc"));  /* 'a' */
  {
    ParseResult(Char) r = runParser(alpha, "abc");
    assert(r.consumed == 1);
    assert(r.succeeded);
    assert(r.ok == 'a');
  }

  // assert(!PARSE_TEST(alpha, ""));    /* error */
  {
    ParseResult(Char) r = runParser(alpha, "");
    assert(r.consumed == 0);
    assert(!r.succeeded);
    assert(r.err.pos.line == 1);
    assert(r.err.pos.column == 1);
  }

  // assert(PARSE_TEST(alnum, "ABC")); /* 'A' */
  {
    ParseResult(Char) r = runParser(alnum, "ABC");
    assert(r.consumed == 1);
    assert(r.succeeded);
    assert(r.ok == 'A');
  }

  // assert(PARSE_TEST(alnum, "BC"));  /* 'B' */
  {
    ParseResult(Char) r = runParser(alnum, "BC");
    assert(r.consumed == 1);
    assert(r.succeeded);
    assert(r.ok == 'B');
  }

  // assert(PARSE_TEST(alnum, "123")); /* '1' */
  {
    ParseResult(Char) r = runParser(alnum, "123");
    assert(r.consumed == 1);
    assert(r.succeeded);
    assert(r.ok == '1');
  }

  // assert(PARSE_TEST(alnum, "abc")); /* 'a' */
  {
    ParseResult(Char) r = runParser(alnum, "abc");
    assert(r.consumed == 1);
    assert(r.succeeded);
    assert(r.ok == 'a');
  }

  // assert(!PARSE_TEST(alnum, ""));   /* error */
  {
    ParseResult(Char) r = runParser(alnum, "");
    assert(r.consumed == 0);
    assert(!r.succeeded);
    assert(r.err.pos.line == 1);
    assert(r.err.pos.column == 1);
  }

  // assert(PARSE_TEST(letter, "ABC"));  /* 'A' */
  {
    ParseResult(Char) r = runParser(letter, "ABC");
    assert(r.consumed == 1);
    assert(r.succeeded);
    assert(r.ok == 'A');
  }

  // assert(PARSE_TEST(letter, "BC"));   /* 'B' */
  {
    ParseResult(Char) r = runParser(letter, "BC");
    assert(r.consumed == 1);
    assert(r.succeeded);
    assert(r.ok == 'B');
  }

  // assert(!PARSE_TEST(letter, "123")); /* error */
  {
    ParseResult(Char) r = runParser(letter, "123");
    assert(r.consumed == 0);
    assert(!r.succeeded);
    assert(r.err.pos.line == 1);
    assert(r.err.pos.column == 1);
  }

  // assert(PARSE_TEST(letter, "abc"));  /* 'a' */
  {
    ParseResult(Char) r = runParser(letter, "abc");
    assert(r.consumed == 1);
    assert(r.succeeded);
    assert(r.ok == 'a');
  }

  // assert(PARSE_TEST(letter, "_+*"));  /* '_' */
  {
    ParseResult(Char) r = runParser(letter, "_+*");
    assert(r.consumed == 1);
    assert(r.succeeded);
    assert(r.ok == '_');
  }

  // assert(!PARSE_TEST(letter, "+*"));  /* error */
  {
    ParseResult(Char) r = runParser(letter, "+*");
    assert(r.consumed == 0);
    assert(!r.succeeded);
    assert(r.err.pos.line == 1);
    assert(r.err.pos.column == 1);
  }

  // assert(!PARSE_TEST(letter, ""));    /* error */
  {
    ParseResult(Char) r = runParser(letter, "");
    assert(r.consumed == 0);
    assert(!r.succeeded);
    assert(r.err.pos.line == 1);
    assert(r.err.pos.column == 1);
  }

  // assert(PARSE_TEST(spaces, "   a"));    /* () */
  {
    ParseResult(None) r = runParser(spaces, "   a");
    assert(r.consumed == 3);
    assert(r.succeeded);
    assert(isNONE(r.ok));
  }

  // assert(PARSE_TEST(spaces, " \t\n\r")); /* () */
  {
    ParseResult(None) r = runParser(spaces, " \t\n\r");
    assert(r.consumed == 4);
    assert(r.succeeded);
    assert(isNONE(r.ok));
  }

  // assert(PARSE_TEST(spaces, "a"));       /* () */
  {
    ParseResult(None) r = runParser(spaces, "a");
    assert(r.consumed == 0);
    assert(r.succeeded);
    assert(isNONE(r.ok));
  }

  // assert(PARSE_TEST(spaces, ""));        /* () */
  {
    ParseResult(None) r = runParser(spaces, "");
    assert(r.consumed == 0);
    assert(r.succeeded);
    assert(isNONE(r.ok));
  }

  // assert(PARSE_TEST(many(anyChar), "12ab!*"));      /* "12ab!*" */
  {
    ParseResult(String) r = runParser(many(anyChar), "12ab!*");
    assert(r.consumed == 6);
    assert(r.succeeded);
    assert(!strcmp(r.ok, "12ab!*"));
  }

  // assert(PARSE_TEST(many(digit), "123"));           /* "123" */
  {
    ParseResult(String) r = runParser(many(digit), "123");
    assert(r.consumed == 3);
    assert(r.succeeded);
    assert(!strcmp(r.ok, "123"));
  }

  // assert(PARSE_TEST(many(digit), "12a"));           /* "12" */
  {
    ParseResult(String) r = runParser(many(digit), "12a");
    assert(r.consumed == 2);
    assert(r.succeeded);
    assert(!strcmp(r.ok, "12"));
  }

  // assert(PARSE_TEST(many(digit), "1234567890abc")); /* "1234567890" */
  {
    ParseResult(String) r = runParser(many(digit), "1234567890abc");
    assert(r.consumed == 10);
    assert(r.succeeded);
    assert(!strcmp(r.ok, "1234567890"));
  }

  // assert(PARSE_TEST(many(digit), ""));              /* "" */
  {
    ParseResult(String) r = runParser(many(digit), "");
    assert(r.consumed == 0);
    assert(r.succeeded);
    assert(!strcmp(r.ok, ""));
  }

  // assert(PARSE_TEST(many(digit), "abc"));           /* "" */
  {
    ParseResult(String) r = runParser(many(digit), "abc");
    assert(r.consumed == 0);
    assert(r.succeeded);
    assert(!strcmp(r.ok, ""));
  }

  // assert(PARSE_TEST(many1(anyChar), "12ab!*"));      /* "12ab!*" */
  {
    ParseResult(String) r = runParser(many1(anyChar), "12ab!*");
    assert(r.consumed == 6);
    assert(r.succeeded);
    assert(!strcmp(r.ok, "12ab!*"));
  }

  // assert(PARSE_TEST(many1(digit), "123"));           /* "123" */
  {
    ParseResult(String) r = runParser(many1(digit), "123");
    assert(r.consumed == 3);
    assert(r.succeeded);
    assert(!strcmp(r.ok, "123"));
  }

  // assert(PARSE_TEST(many1(digit), "12a"));           /* "12" */
  {
    ParseResult(String) r = runParser(many1(digit), "12a");
    assert(r.consumed == 2);
    assert(r.succeeded);
    assert(!strcmp(r.ok, "12"));
  }

  // assert(PARSE_TEST(many1(digit), "1234567890abc")); /* "1234567890" */
  {
    ParseResult(String) r = runParser(many1(digit), "1234567890abc");
    assert(r.consumed == 10);
    assert(r.succeeded);
    assert(!strcmp(r.ok, "1234567890"));
  }

  // assert(!PARSE_TEST(many1(digit), ""));             /* error */
  {
    ParseResult(String) r = runParser(many1(digit), "");
    assert(r.consumed == 0);
    assert(!r.succeeded);
    assert(r.err.pos.line == 1);
    assert(r.err.pos.column == 1);
  }

  // assert(!PARSE_TEST(many1(digit), "abc"));          /* error */
  {
    ParseResult(String) r = runParser(many1(digit), "abc");
    assert(r.consumed == 0);
    assert(!r.succeeded);
    assert(r.err.pos.line == 1);
    assert(r.err.pos.column == 1);
  }

  // assert(PARSE_TEST(seq(letter, digit, digit), "a12"));  /* "a12" */
  {
    ParseResult(String) r = runParser(seq(letter, digit, digit), "a12");
    assert(r.consumed == 3);
    assert(r.succeeded);
    assert(!strcmp(r.ok, "a12"));
  }

  // assert(PARSE_TEST(seq(letter, digit, digit), "_123")); /* "_12" */
  {
    ParseResult(String) r = runParser(seq(letter, digit, digit), "_123");
    assert(r.consumed == 3);
    assert(r.succeeded);
    assert(!strcmp(r.ok, "_12"));
  }

  // assert(!PARSE_TEST(seq(letter, digit, digit), "123")); /* error */
  {
    ParseResult(String) r = runParser(seq(letter, digit, digit), "123");
    assert(r.consumed == 0);
    assert(!r.succeeded);
    assert(r.err.pos.line == 1);
    assert(r.err.pos.column == 1);
  }

  // assert(PARSE_TEST(cons(letter, many(digit)), "a1234"));  /* "a1234"
  // */
  {
    ParseResult(String) r = runParser(cons(letter, many(digit)), "a1234");
    assert(r.consumed == 5);
    assert(r.succeeded);
    assert(!strcmp(r.ok, "a1234"));
  }

  // assert(PARSE_TEST(cons(letter, many(digit)), "abc123")); /* "a" */
  {
    ParseResult(String) r =
        runParser(cons(letter, many(digit)), "abc123");
    assert(r.consumed == 1);
    assert(r.succeeded);
    assert(!strcmp(r.ok, "a"));
  }

  // assert(PARSE_TEST(cons(letter, many(digit)), "a123bc")); /* "a123" */
  {
    ParseResult(String) r =
        runParser(cons(letter, many(digit)), "a123bc");
    assert(r.consumed == 4);
    assert(r.succeeded);
    assert(!strcmp(r.ok, "a123"));
  }

  // assert(PARSE_TEST(string1("a1234"), "a1234"));   /* "a1234" */
  {
    ParseResult(String) r = runParser(string1("a1234"), "a1234");
    assert(r.consumed == 5);
    assert(r.succeeded);
    assert(!strcmp(r.ok, "a1234"));
  }

  // assert(PARSE_TEST(string1("a123"), "a1234"));    /* "a123" */
  {
    ParseResult(String) r = runParser(string1("a123"), "a1234");
    assert(r.consumed == 4);
    assert(r.succeeded);
    assert(!strcmp(r.ok, "a123"));
  }

  // assert(PARSE_TEST(string1("a123"), "a123bc"));   /* "a123" */
  {
    ParseResult(String) r = runParser(string1("a123"), "a123bc");
    assert(r.consumed == 4);
    assert(r.succeeded);
    assert(!strcmp(r.ok, "a123"));
  }

  // assert(!PARSE_TEST(string1("a1234"), "a123"));   /* error */
  {
    ParseResult(String) r = runParser(string1("a1234"), "a123");
    assert(r.consumed == 0);
    assert(!r.succeeded);
    assert(r.err.pos.line == 1);
    assert(r.err.pos.column == 1);
  }

  // assert(!PARSE_TEST(string1("a1234"), "a123bc")); /* error */
  {
    ParseResult(String) r = runParser(string1("a1234"), "a123bc");
    assert(r.consumed == 0);
    assert(!r.succeeded);
    assert(r.err.pos.line == 1);
    assert(r.err.pos.column == 1);
  }

  PARSER(String) digit3 = PARSER_GEN(String)(run_digit3, NULL);

  // assert(PARSE_TEST(digit3, "1234"));  /* "123" */
  {
    ParseResult(String) r = runParser(digit3, "1234");
    assert(r.consumed == 3);
    assert(r.succeeded);
    assert(!strcmp(r.ok, "123"));
  }

  // assert(PARSE_TEST(digit3, "123"));   /* "123" */
  {
    ParseResult(String) r = runParser(digit3, "123");
    assert(r.consumed == 3);
    assert(r.succeeded);
    assert(!strcmp(r.ok, "123"));
  }

  // assert(!PARSE_TEST(digit3, "12"));   /* error */
  {
    ParseResult(String) r = runParser(digit3, "12");
    assert(r.consumed == 2);
    assert(!r.succeeded);
    assert(r.err.pos.line == 1);
    assert(r.err.pos.column == 3);
  }

  // assert(!PARSE_TEST(digit3, "a123")); /* error */
  {
    ParseResult(String) r = runParser(digit3, "a123");
    assert(r.consumed == 0);
    assert(!r.succeeded);
    assert(r.err.pos.line == 1);
    assert(r.err.pos.column == 1);
  }

  // assert(PARSE_TEST(cons(upper, digit3), "A123"));  /* "A123" */
  {
    ParseResult(String) r = runParser(cons(upper, digit3), "A123");
    assert(r.consumed == 4);
    assert(r.succeeded);
    assert(!strcmp(r.ok, "A123"));
  }

  // assert(!PARSE_TEST(cons(upper, digit3), "a123")); /* error */
  {
    ParseResult(String) r = runParser(cons(upper, digit3), "a123");
    assert(r.consumed == 0);
    assert(!r.succeeded);
    assert(r.err.pos.line == 1);
    assert(r.err.pos.column == 1);
  }

  // assert(PARSE_TEST(number, "1234"));  /* 1234 */
  {
    ParseResult(Int) r = runParser(number, "1234");
    assert(r.consumed == 4);
    assert(r.succeeded);
    assert(r.ok == 1234);
  }

  // assert(PARSE_TEST(number, "123"));   /* 123 */
  {
    ParseResult(Int) r = runParser(number, "123");
    assert(r.consumed == 3);
    assert(r.succeeded);
    assert(r.ok == 123);
  }

  // assert(PARSE_TEST(number, "12"));    /* 12 */
  {
    ParseResult(Int) r = runParser(number, "12");
    assert(r.consumed == 2);
    assert(r.succeeded);
    assert(r.ok == 12);
  }

  // assert(PARSE_TEST(number, "1"));     /* 1 */
  {
    ParseResult(Int) r = runParser(number, "1");
    assert(r.consumed == 1);
    assert(r.succeeded);
    assert(r.ok == 1);
  }

  // assert(!PARSE_TEST(number, ""));     /* error */
  {
    ParseResult(Int) r = runParser(number, "");
    assert(r.consumed == 0);
    assert(!r.succeeded);
    assert(r.err.pos.line == 1);
    assert(r.err.pos.column == 1);
  }

  // assert(!PARSE_TEST(number, "a123")); /* error */
  {
    ParseResult(Int) r = runParser(number, "a123");
    assert(r.consumed == 0);
    assert(!r.succeeded);
    assert(r.err.pos.line == 1);
    assert(r.err.pos.column == 1);
  }

  // assert(!PARSE_TEST(token((char)'+'), "1 + 2")); /* error */
  {
    ParseResult(Char) r = runParser(token((char)'+'), "1 + 2");
    assert(r.consumed == 0);
    assert(!r.succeeded);
    assert(r.err.pos.line == 1);
    assert(r.err.pos.column == 1);
  }

  // assert(PARSE_TEST(token((char)'+'), "+ 2"));    /* '+' */
  {
    ParseResult(Char) r = runParser(token((char)'+'), "+ 2");
    assert(r.consumed == 2);
    assert(r.succeeded);
    assert(r.ok = '+');
  }

  // assert(!PARSE_TEST(token((char)'+'), "2"));     /* error */
  {
    ParseResult(Char) r = runParser(token((char)'+'), "2");
    assert(r.consumed == 0);
    assert(!r.succeeded);
    assert(r.err.pos.line == 1);
    assert(r.err.pos.column == 1);
  }

  // assert(PARSE_TEST(either("ab", "bc"), "ab"));  /* "ab" */
  {
    ParseResult(String) r = runParser(either("ab", "bc"), "ab");
    assert(r.consumed == 2);
    assert(r.succeeded);
    assert(!strcmp(r.ok, "ab"));
  }

  // assert(PARSE_TEST(either("ab", "bc"), "bc"));  /* "bc" */
  {
    ParseResult(String) r = runParser(either("ab", "bc"), "bc");
    assert(r.consumed == 2);
    assert(r.succeeded);
    assert(!strcmp(r.ok, "bc"));
  }

  // assert(!PARSE_TEST(either("ab", "bc"), "ac")); /* error */
  {
    ParseResult(String) r = runParser(either("ab", "bc"), "ac");
    assert(r.consumed == 0);
    assert(!r.succeeded);
    assert(r.err.pos.line == 1);
    assert(r.err.pos.column == 1);
  }

  // assert(PARSE_TEST(either("ab", "ac"), "ac")); /* "ac" */
  {
    ParseResult(String) r = runParser(either("ab", "ac"), "ac");
    assert(r.consumed == 2);
    assert(r.succeeded);
    assert(!strcmp(r.ok, "ac"));
  }

  PARSER(Char) a = char1('a');
  PARSER(Char) b = char1('b');
  PARSER(Char) c = char1('c');

  // assert(PARSE_TEST(either(seq(a, b), seq(a, c)), "ab"));  /* "ab" */
  {
    ParseResult(String) r = runParser(either(seq(a, b), seq(a, c)), "ab");
    assert(r.consumed == 2);
    assert(r.succeeded);
    assert(!strcmp(r.ok, "ab"));
  }

  // assert(!PARSE_TEST(either(seq(a, b), seq(a, c)), "ac")); /* error */
  {
    ParseResult(String) r = runParser(either(seq(a, b), seq(a, c)), "ac");
    assert(r.consumed == 1);
    assert(!r.succeeded);
    assert(r.err.pos.line == 1);
    assert(r.err.pos.column == 2);
  }

  // assert(PARSE_TEST(either(tryp(seq(a, b)), seq(a, c)), "ac"));
  // /* -> "ac" */
  {
    ParseResult(String) r =
        runParser(either(tryp(seq(a, b)), seq(a, c)), "ac");
    assert(r.consumed == 2);
    assert(r.succeeded);
    assert(!strcmp(r.ok, "ac"));
  }

  // assert(!PARSE_TEST(either("ab", "cd"), "xy")); /* error */
  {
    ParseResult(String) r = runParser(either("ab", "cd"), "xy");
    assert(r.consumed == 0);
    assert(!r.succeeded);
    assert(r.err.pos.line == 1);
    assert(r.err.pos.column == 1);
  }

  // assert(PARSE_TEST(FOLDL(either, "ab", "cd", "ef"), "ab")); /* "ab" */
  {
    ParseResult(String) r =
        runParser(FOLDL(either, "ab", "cd", "ef"), "ab");
    assert(r.consumed == 2);
    assert(r.succeeded);
    assert(!strcmp(r.ok, "ab"));
  }

  // assert(PARSE_TEST(FOLDL(either, "ab", "cd", "ef"), "cd"));  /* "cd"
  // */
  {
    ParseResult(String) r =
        runParser(FOLDL(either, "ab", "cd", "ef"), "cd");
    assert(r.consumed == 2);
    assert(r.succeeded);
    assert(!strcmp(r.ok, "cd"));
  }

  // assert(PARSE_TEST(FOLDL(either, "ab", "cd", "ef"), "ef"));  /* "ef"
  // */
  {
    ParseResult(String) r =
        runParser(FOLDL(either, "ab", "cd", "ef"), "ef");
    assert(r.consumed == 2);
    assert(r.succeeded);
    assert(!strcmp(r.ok, "ef"));
  }

  // assert(!PARSE_TEST(FOLDL(either, "ab", "cd", "ef"), "xy")); /* error
  // */
  {
    ParseResult(String) r =
        runParser(FOLDL(either, "ab", "cd", "ef"), "xy");
    assert(r.consumed == 0);
    assert(!r.succeeded);
    assert(r.err.pos.line == 1);
    assert(r.err.pos.column == 1);
  }

  // assert(PARSE_TEST(skip((char)'a'), "ab"));         /* () */
  {
    ParseResult(None) r = runParser(skip((char)'a'), "ab");
    assert(r.consumed == 1);
    assert(r.succeeded);
    assert(isNONE(r.ok));
  }

  // assert(PARSE_TEST(many(skip((char)'a')), "aabb")); /* [(), ()] */
  {
    ParseResult(List(None)) r = runParser(many(skip((char)'a')), "aabb");
    assert(r.consumed == 2);
    assert(r.succeeded);
    assert(list_length(r.ok) == 2);
    assert(isNONE(list_begin(r.ok)[0]));
    assert(isNONE(list_begin(r.ok)[1]));
  }

  // assert(PARSE_TEST(many(skip(either((char)'a', (char)'b'))), "aabb"));
  // /* -> [(), (), (), ()]  */
  {
    ParseResult(List(None)) r =
        runParser(many(skip(either((char)'a', (char)'b'))), "aabb");
    assert(r.consumed == 4);
    assert(r.succeeded);
    assert(list_length(r.ok) == 4);
    assert(isNONE(list_begin(r.ok)[0]));
    assert(isNONE(list_begin(r.ok)[1]));
    assert(isNONE(list_begin(r.ok)[2]));
    assert(isNONE(list_begin(r.ok)[3]));
  }

  // assert(PARSE_TEST(many(number), "123 456 789"));
  // /* -> [123, 456, 789] */
  {
    ParseResult(List(Int)) r = runParser(many(number), "123 456 789");
    assert(r.consumed == 11);
    assert(r.succeeded);
    assert(list_length(r.ok) == 3);
    assert(list_begin(r.ok)[0] == 123);
    assert(list_begin(r.ok)[1] == 456);
    assert(list_begin(r.ok)[2] == 789);
  }

  // assert(PARSE_TEST(many(number), ""));
  // /* -> [] */
  {
    ParseResult(List(Int)) r = runParser(many(number), "");
    assert(r.consumed == 0);
    assert(r.succeeded);
    assert(list_length(r.ok) == 0);
  }

  // assert(PARSE_TEST(many(token(many1(digit))), "123 456 789"));
  // /* -> ["123", "456", "789"] */
  {
    ParseResult(List(String)) r =
        runParser(many(token(many1(digit))), "123 456 789");
    assert(r.consumed == 11);
    assert(r.succeeded);
    assert(list_length(r.ok) == 3);
    assert(!strcmp(list_begin(r.ok)[0], "123"));
    assert(!strcmp(list_begin(r.ok)[1], "456"));
    assert(!strcmp(list_begin(r.ok)[2], "789"));
  }

  // assert(PARSE_TEST(many(token(many1(digit))), ""));
  // /* -> [] */
  {
    ParseResult(List(String)) r =
        runParser(many(token(many1(digit))), "");
    assert(r.consumed == 0);
    assert(r.succeeded);
    assert(list_length(r.ok) == 0);
  }

  // assert(PARSE_TEST(many(skip1st(char1(','), number)),
  // ",123,456,789"));
  // /* -> [123, 456, 789] */
  {
    ParseResult(List(Int)) r =
        runParser(many(skip1st(char1(','), number)), ",123,456,789");
    assert(r.consumed == 12);
    assert(r.succeeded);
    assert(list_length(r.ok) == 3);
    assert(list_begin(r.ok)[0] == 123);
    assert(list_begin(r.ok)[1] == 456);
    assert(list_begin(r.ok)[2] == 789);
  }

  // assert(PARSE_TEST(many(skip1st(char1(','), many1(digit))),
  // ",123,456,789"));
  // /* -> ["123", "456", "789"] */
  {
    ParseResult(List(String)) r = runParser(
        many(skip1st(char1(','), many1(digit))), ",123,456,789");
    assert(r.consumed == 12);
    assert(r.succeeded);
    assert(list_length(r.ok) == 3);
    assert(!strcmp(list_begin(r.ok)[0], "123"));
    assert(!strcmp(list_begin(r.ok)[1], "456"));
    assert(!strcmp(list_begin(r.ok)[2], "789"));
  }

  // assert(!PARSE_TEST(many(endOfFile), ""));       /* error */
  {
    ParseResult(List(None)) r = runParser(many(endOfFile), "");
    assert(r.consumed == 0);
    assert(!r.succeeded);
    assert(r.err.pos.line == 1);
    assert(r.err.pos.column == 1);
  }

  // assert(!PARSE_TEST(many(spaces), "aaa"));       /* error */
  {
    ParseResult(List(None)) r = runParser(many(spaces), "aaa");
    assert(r.consumed == 0);
    assert(!r.succeeded);
    assert(r.err.pos.line == 1);
    assert(r.err.pos.column == 1);
  }

  // assert(!PARSE_TEST(many(skip(endOfFile)), "")); /* error */
  {
    ParseResult(List(None)) r = runParser(many(skip(endOfFile)), "");
    assert(r.consumed == 0);
    assert(!r.succeeded);
    assert(r.err.pos.line == 1);
    assert(r.err.pos.column == 1);
  }

  // assert(!PARSE_TEST(many(skip(spaces)), "aaa")); /* error */
  {
    ParseResult(List(None)) r = runParser(many(skip(spaces)), "aaa");
    assert(r.consumed == 0);
    assert(!r.succeeded);
    assert(r.err.pos.line == 1);
    assert(r.err.pos.column == 1);
  }

  // assert(!PARSE_TEST(many1(endOfFile), ""));       /* error */
  {
    ParseResult(List(None)) r = runParser(many1(endOfFile), "");
    assert(r.consumed == 0);
    assert(!r.succeeded);
    assert(r.err.pos.line == 1);
    assert(r.err.pos.column == 1);
  }

  // assert(!PARSE_TEST(many1(spaces), "aaa"));       /* error */
  {
    ParseResult(List(None)) r = runParser(many1(spaces), "aaa");
    assert(r.consumed == 0);
    assert(!r.succeeded);
    assert(r.err.pos.line == 1);
    assert(r.err.pos.column == 1);
  }

  // assert(!PARSE_TEST(many1(skip(endOfFile)), "")); /* error */
  {
    ParseResult(List(None)) r = runParser(many1(skip(endOfFile)), "");
    assert(r.consumed == 0);
    assert(!r.succeeded);
    assert(r.err.pos.line == 1);
    assert(r.err.pos.column == 1);
  }

  // assert(!PARSE_TEST(many1(skip(spaces)), "aaa")); /* error */
  {
    ParseResult(List(None)) r = runParser(many1(skip(spaces)), "aaa");
    assert(r.consumed == 0);
    assert(!r.succeeded);
    assert(r.err.pos.line == 1);
    assert(r.err.pos.column == 1);
  }

  cparsec2_end();
}
