/* -*- coding:utf-8-unix -*- */

#include "cparsec2.h"

#define SKIP1ST_FN(T) run_skip1st##T
#define DEFINE_SKIP1ST(T)                                                \
  static RETURN_TYPE(PARSER(T))                                          \
      SKIP1ST_FN(T)(void* arg, Source src, Ctx* ex) {                    \
    void** ps = (void**)arg;                                             \
    parse((PARSER(None))ps[0], src, ex);                                 \
    return parse((PARSER(T))ps[1], src, ex);                             \
  }                                                                      \
  PARSER(T) SKIP1ST(T)(PARSER(None) p1, PARSER(T) p2) {                  \
    void** arg = mem_malloc(sizeof(void*) * 2);                          \
    arg[0] = (void*)p1;                                                  \
    arg[1] = (void*)p2;                                                  \
    return PARSER_GEN(T)(SKIP1ST_FN(T), (void*)arg);                     \
  }                                                                      \
  END_OF_STATEMENTS

FOREACH(DEFINE_SKIP1ST, TYPESET(1));
