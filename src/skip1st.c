/* -*- coding:utf-8-unix -*- */

#include "cparsec2.h"

#define SKIP1ST_FN(T) run_skip##T
#define DEFINE_SKIP1ST(T, R)                                             \
  static R SKIP1ST_FN(T)(void* arg, Source src, Ctx* ex) {               \
    void** ps = (void**)arg;                                             \
    parse((IntParser)ps[0], src, ex);                                    \
    return parse((PARSER(T))ps[1], src, ex);                             \
  }                                                                      \
  PARSER(T) SKIP1ST(T)(IntParser p1, PARSER(T) p2) {                     \
    void** arg = mem_malloc(sizeof(void*) * 2);                          \
    arg[0] = (void*)p1;                                                  \
    arg[1] = (void*)p2;                                                  \
    return PARSER_GEN(T)(SKIP1ST_FN(T), (void*)arg);                     \
  }                                                                      \
  _Static_assert(1, "")

DEFINE_SKIP1ST(Char, char);
DEFINE_SKIP1ST(String, const char*);
DEFINE_SKIP1ST(Int, int);
