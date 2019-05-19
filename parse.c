/* -*- coding:utf-8-unix -*- */

#include "cparsec2.h"

const char *error(const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  int len = vsnprintf(NULL, 0, fmt, ap);
  if (len < 0) {
    fprintf(stderr, "vsnprintf(NULL, 0, fmt, ...):%s\n", strerror(errno));
    exit(1);
  }
  char *buf = malloc(len + 1);
  if (vsnprintf(buf, len + 1, fmt, ap) < 0) {
    fprintf(stderr, "vsnprintf(buf, len, fmt, ...):%s\n", strerror(errno));
    exit(1);
  }
  return buf;
}

CharResult peek(Source src) {
  char c = *src->p;
  if (!c) {
    return (CharResult){.error = error("too short")};
  }
  return (CharResult){.result = c};
}

void consume(Source src) {
  assert(*src->p);
  src->p++;
}

CharParser anyChar;
CharParser digit;
CharParser lower;
CharParser upper;
CharParser alpha;
CharParser alnum;
CharParser letter;

void cparsec2_init(void) {
  anyChar = satisfy(is_anyChar);
  digit   = satisfy(is_digit);
  lower   = satisfy(is_lower);
  upper   = satisfy(is_upper);
  alpha   = satisfy(is_alpha);
  alnum   = satisfy(is_alnum);
  letter  = satisfy(is_letter);
}

// void show(T x);
#define show(x) _Generic((x), char : show_char, const char * : show_string)(x)

void show_char(char c) { printf("'%c'\n", c); }
void show_string(const char *str) { printf("\"%s\"\n", str); }

void parseTest_char(CharParser p, const char* input) {
  struct stSource src = {.input = input, .p = input};
  CharResult x = parse(p, &src);
  if (x.error) {
    printf("error:%s\n", x.error);
  } else {
    show(x.result);
  }
}

void parseTest_string(StringParser p, const char* input) {
  struct stSource src = {.input = input, .p = input};
  StringResult x = parse(p, &src);
  if (x.error) {
    printf("error:%s\n", x.error);
  } else {
    show(x.result);
  }
}
