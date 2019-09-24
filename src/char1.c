/* -*- coding:utf-8-unix -*- */

#include "cparsec2.h"

#include <ctype.h>

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
  case '\0':                    /* Null */
    return "'\\0'";
  case '\a':                    /* Bell */
    return "'\\a'";
  case '\b':                    /* Backspace */
    return "'\\b'";
  case '\t':                    /* Horizontal Tab */
    return "'\\t'";
  case '\n':                    /* Line Feed */
    return "'\\n'";
  case '\v':                    /* Vertical Tab */
    return "'\\v'";
  case '\f':                    /* Form Feed */
    return "'\\f'";
  case '\r':                    /* Carriage Return */
    return "'\\r'";
  case '\\':                    /* Backslash */
    return "'\\\\'";
  default:;
    const char** p = &(char1_cache.str[c]);
    if (!*p) {
      if (isprint(c)) {
        *p = mem_printf("'%c'", c);
      }
      else {
        *p = mem_printf("'\\%03o'", (unsigned int)c);
      }
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
  ErrMsg m = {SysUnexpect, c2s(c)};
  parseError(src, m);
  cthrow(ex, error("expects %s but was %s", c2s(expected), c2s(c)));
}

PARSER(Char) char1(char c) {
  PARSER(Char)* p = &(char1_cache.parser[(uint8_t)c]);
  if (!*p) {
    *p = expects(c2s(c), PARSER_GEN(Char)(run_char1, (void*)(intptr_t)c));
  }
  return *p;
}
