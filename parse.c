/* -*- coding:utf-8-unix -*- */

#include "cparsec2.h"

const char *error(const char *fmt, ...) {
  char buf[1024] = {0};
  va_list ap;
  va_start(ap, fmt);
  vsprintf(buf, fmt, ap);
  return strdup(buf);
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
