/* -*- coding:utf-8-unix -*- */
#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdnoreturn.h>
#include <string.h>

#define UNUSED(v)    ((void)v)

/** Print error message then exit with error code. */
noreturn void error(const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  fprintf(stderr, "error:");
  vfprintf(stderr, fmt, ap);
  fprintf(stderr, "\n");
  exit(1);
}

typedef char (*Parser)(const char* input);

void parseTest(Parser p, const char* input)
{
  char c = p(input);
  printf("\"%s\" => '%c'\n", input, c);
}

char anyChar(const char* input) {
  char c = *input;
  if (!c) {
    error("too short");
  }
  return c;
}

int main(int argc, char **argv) {
  UNUSED(argc);
  UNUSED(argv);

  parseTest(anyChar, "abc");    /* a */
  parseTest(anyChar, "bc");     /* b */
  parseTest(anyChar, "123");    /* 1 */
  parseTest(anyChar, "");       /* "error:too short" */

  return 0;
}
