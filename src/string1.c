/* -*- coding:utf-8-unix -*- */

#include "cparsec2.h"

static const char* run_string1(void* arg, Source src, Ctx* ex) {
  const char* expected = (const char*)arg;
  const char* p = expected;
  while (*p) {
    parse(char1(*p), src, ex);
    p++;
  }
  return expected;
}

PARSER(String) string1(const char* s) {
  return expects(mem_printf("\"%s\"", s),
                 PARSER_GEN(String)(run_string1, (void*)s));
}
