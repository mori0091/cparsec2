/* -*- coding:utf-8-unix -*- */

#include "cparsec2.h"

#define SKIP_FN(T) run_skip##T
#define DEFINE_SKIP(T)                                                   \
  static None SKIP_FN(T)(void* arg, Source src, Ctx* ex) {               \
    PARSER(T) p = (PARSER(T))arg;                                        \
    parse(p, src, ex);                                                   \
    return NONE;                                                         \
  }                                                                      \
  PARSER(None) SKIP(T)(PARSER(T) p) {                                    \
    return PARSER_GEN(None)(SKIP_FN(T), (void*)p);                       \
  }                                                                      \
  END_OF_STATEMENTS

FOREACH(DEFINE_SKIP, TYPESET(1));
