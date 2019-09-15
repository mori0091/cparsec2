/* -*- coding:utf-8-unix -*- */

#include "cparsec2.h"

#define SKIP2ND_FN(T) run_skip2nd##T
#define DEFINE_SKIP2ND(T)                                                \
  static RETURN_TYPE(PARSER(T))                                          \
      SKIP2ND_FN(T)(void* arg, Source src, Ctx* ex) {                    \
    void** ps = (void**)arg;                                             \
    RETURN_TYPE(PARSER(T)) ret = parse((PARSER(T))ps[0], src, ex);       \
    parse((PARSER(None))ps[1], src, ex);                                 \
    return ret;                                                          \
  }                                                                      \
  PARSER(T) SKIP2ND(T)(PARSER(T) p1, PARSER(None) p2) {                  \
    void** arg = mem_malloc(sizeof(void*) * 2);                          \
    arg[0] = (void*)p1;                                                  \
    arg[1] = (void*)p2;                                                  \
    return PARSER_GEN(T)(SKIP2ND_FN(T), (void*)arg);                     \
  }                                                                      \
  END_OF_STATEMENTS

FOREACH(DEFINE_SKIP2ND, TYPESET(1));
