/* -*- coding:utf-8-unix -*- */

#include "cparsec2.h"

#define SKIP_FN(T) run_skip##T
#define DEFINE_SKIP(T)                                                   \
  static int SKIP_FN(T)(void* arg, Source src, Ctx* ex) {                \
    PARSER(T) p = (PARSER(T))arg;                                        \
    parse(p, src, ex);                                                   \
    return 0;                                                            \
  }                                                                      \
  PARSER(Int) SKIP(T)(PARSER(T) p) {                                     \
    return PARSER_GEN(Int)(SKIP_FN(T), (void*)p);                        \
  }                                                                      \
  _Static_assert(1, "")

DEFINE_SKIP(Char);
DEFINE_SKIP(String);
DEFINE_SKIP(Int);
