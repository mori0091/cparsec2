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


typedef struct stParser *Parser;
struct stParser {
  char (*run)(Parser self, Source src);
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

// ----

int main(int argc, char **argv) {
  UNUSED(argc);
  UNUSED(argv);

  PARSE_TEST(anyChar, "abc"); /* a */
  PARSE_TEST(anyChar, "bc");  /* b */
  PARSE_TEST(anyChar, "123"); /* 1 */
  PARSE_TEST(anyChar, "");    /* "error:too short" */

  return 0;
}
