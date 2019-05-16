/* -*- coding:utf-8-unix -*- */
#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define UNUSED(v) ((void)v)

/** Print error message and return '\0' */
char error(const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  printf("error:");
  vprintf(fmt, ap);
  printf("\n");
  return '\0';
}

typedef struct stSource *Source;
struct stSource {
  const char *input; /* whole input */
  const char *p;     /* pointer to next char */
};

typedef bool (*Predicate)(char c);
bool is_digit(char c) { return '0' <= c && c <= '9'; }
bool is_lower(char c) { return 'a' <= c && c <= 'z'; }
bool is_upper(char c) { return 'A' <= c && c <= 'Z'; }
bool is_alpha(char c) { return is_lower(c) || is_upper(c); }
bool is_alnum(char c) { return is_digit(c) || is_alpha(c); }
bool is_letter(char c) { return c == '_' || is_alpha(c); }

typedef struct stParser *Parser;
struct stParser {
  char (*run)(Parser self, Source src);
  union {
    Predicate pred;
  };
};

#define PARSE_TEST(p, input)                                                   \
  printf("%8s \"%s\" => ", #p, input);                                         \
  parseTest(p, input)

char parse(Parser p, Source src) { return p->run(p, src); }

void parseTest(Parser p, const char *input) {
  struct stSource src;
  src.input = input;
  src.p = input;
  char c = parse(p, &src);
  if (c) {
    printf("'%c'\n", c);
  }
}

extern Parser const anyChar;
Parser satisfy(Predicate pred);

// anyChar

static char run_anyChar(Parser p, Source src) {
  UNUSED(p);
  char c = *src->p;
  if (!c) {
    return error("too short");
  }
  return c;
}

static struct stParser anyChar_ = {
    .run = run_anyChar,
};

Parser const anyChar = &anyChar_;

// satisfy

static char run_satisfy(Parser p, Source src) {
  char c = parse(anyChar, src);
  if (!c) {
    return c; /* error */
  }
  if (!p->pred(c)) {
    return error("not satisfy");
  }
  return c;
}

Parser satisfy(Predicate pred) {
  Parser p = malloc(sizeof(struct stParser));
  p->run = run_satisfy;
  p->pred = pred;
  return p;
}

// ----

int main(int argc, char **argv) {
  UNUSED(argc);
  UNUSED(argv);

  PARSE_TEST(anyChar, "abc"); /* a */
  PARSE_TEST(anyChar, "bc");  /* b */
  PARSE_TEST(anyChar, "123"); /* 1 */
  PARSE_TEST(anyChar, "");    /* "error:too short" */

  Parser digit = satisfy(is_digit);
  PARSE_TEST(digit, "123"); /* 1 */
  PARSE_TEST(digit, "23");  /* 2 */
  PARSE_TEST(digit, "abc"); /* "error:not satisfy" */
  PARSE_TEST(digit, "");    /* "error:too short" */

  Parser lower = satisfy(is_lower);
  PARSE_TEST(lower, "abc"); /* a */
  PARSE_TEST(lower, "bc");  /* b */
  PARSE_TEST(lower, "123"); /* "error:not satisfy" */
  PARSE_TEST(lower, "ABC"); /* "error:not satisfy" */
  PARSE_TEST(lower, "");    /* "error:too short" */

  Parser upper = satisfy(is_upper);
  PARSE_TEST(upper, "ABC"); /* A */
  PARSE_TEST(upper, "BC");  /* B */
  PARSE_TEST(upper, "123"); /* "error:not satisfy" */
  PARSE_TEST(upper, "abc"); /* "error:not satisfy" */
  PARSE_TEST(upper, "");    /* "error:too short" */

  Parser alpha = satisfy(is_alpha);
  PARSE_TEST(alpha, "ABC"); /* A */
  PARSE_TEST(alpha, "BC");  /* B */
  PARSE_TEST(alpha, "123"); /* "error:not satisfy" */
  PARSE_TEST(alpha, "abc"); /* a */
  PARSE_TEST(alpha, "");    /* "error:too short" */

  Parser alnum = satisfy(is_alnum);
  PARSE_TEST(alnum, "ABC"); /* A */
  PARSE_TEST(alnum, "BC");  /* B */
  PARSE_TEST(alnum, "123"); /* 1 */
  PARSE_TEST(alnum, "abc"); /* a */
  PARSE_TEST(alnum, "");    /* "error:too short" */

  Parser letter = satisfy(is_letter);
  PARSE_TEST(letter, "ABC"); /* A */
  PARSE_TEST(letter, "BC");  /* B */
  PARSE_TEST(letter, "123"); /* "error:not satisfy" */
  PARSE_TEST(letter, "abc"); /* a */
  PARSE_TEST(letter, "_+*"); /* _ */
  PARSE_TEST(letter, "+*");  /* "error:not satisfy" */
  PARSE_TEST(letter, "");    /* "error:too short" */

  return 0;
}
