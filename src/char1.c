/* -*- coding:utf-8-unix -*- */

#include "cparsec2.h"

static const char* c2s(uint8_t c) {
  switch (c) {
  case '\n':
    return "'\\n'";
  case '\r':
    return "'\\r'";
  case '\t':
    return "'\\t'";
  case ' ':
    return "' '";
  default:
    return mem_printf("'%c'", c);
  }
}

static char run_char1(void* arg, Source src, Ctx* ex) {
  char expected = (char)(intptr_t)arg;
  char c = peek(src, ex);
  if (expected == c) {
    consume(src);
    return c;
  }
  parseError(src, (ErrMsg){Unexpect, c2s(c)});
  cthrow(ex, error("expects %s but was %s", c2s(expected), c2s(c)));
}

PARSER(Char) char1(char c) {
  return expects(c2s(c), PARSER_GEN(Char)(run_char1, (void*)(intptr_t)c));
}
