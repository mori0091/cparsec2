/* -*- coding:utf-8-unix -*- */

#include "cparsec2.h"

struct char1_cache {
  PARSER(Char) parser[256];
  const char* str[256];
};

static struct char1_cache char1_cache = {0};

void cparsec2_clear_char1_cache(void) {
  char1_cache = (struct char1_cache){0};
}

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
  default:;
    const char** p = &(char1_cache.str[c]);
    if (!*p) {
      *p = mem_printf("'%c'", c);
    }
    return *p;
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
  PARSER(Char)* p = &(char1_cache.parser[(uint8_t)c]);
  if (!*p) {
    *p = expects(c2s(c), PARSER_GEN(Char)(run_char1, (void*)(intptr_t)c));
  }
  return *p;
}
