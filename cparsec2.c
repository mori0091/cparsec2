/* -*- coding:utf-8-unix -*- */
#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdnoreturn.h>
#include <string.h>

#define UNUSED(v) ((void)v)

/** Print error message then exit with error code. */
noreturn void error(const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  fprintf(stderr, "\nerror:");
  vfprintf(stderr, fmt, ap);
  fprintf(stderr, "\n");
  exit(1);
}

typedef struct stSource {
  const char *input; /* whole input */
  const char *p;     /* pointer to next char */
} * Source;

typedef char (*Parser)(Source src);

void parseTest(Parser p, const char *input) {
  struct stSource src;
  src.input = input;
  src.p = input;
  printf("\"%s\"", input);
  char c = p(&src);
  printf(" => '%c'\n", c);
}

char anyChar(Source src) {
  char c = *src->p;
  if (!c) {
    error("too short");
  }
  return c;
}

int main(int argc, char **argv) {
  UNUSED(argc);
  UNUSED(argv);

  parseTest(anyChar, "abc"); /* a */
  parseTest(anyChar, "bc");  /* b */
  parseTest(anyChar, "123"); /* 1 */
  parseTest(anyChar, "");    /* "error:too short" */

  return 0;
}
