/* -*- coding:utf-8-unix -*- */

#include "cparsec2.h"

#define EITHER_FN(T) run_either##T
#define DEFINE_EITHER(T)                                                 \
  static RETURN_TYPE(PARSER(T))                                          \
      EITHER_FN(T)(void* arg, Source src, Ctx* ex) {                     \
    PARSER(T)* p = (PARSER(T)*)arg;                                      \
    SourcePos pos = getSourcePos(src);                                   \
    Ctx ctx;                                                             \
    TRY(&ctx) {                                                          \
      return parse(p[0], src, &ctx);                                     \
    }                                                                    \
    else if (isSourcePosEqual(pos, getSourcePos(src))) {                 \
      return parse(p[1], src, ex);                                       \
    }                                                                    \
    else {                                                               \
      cthrow(ex, ctx.msg);                                               \
    }                                                                    \
  }                                                                      \
  PARSER(T) EITHER(T)(PARSER(T) p1, PARSER(T) p2) {                      \
    PARSER(T)* arg = mem_malloc(sizeof(PARSER(T)[2]));                   \
    arg[0] = p1;                                                         \
    arg[1] = p2;                                                         \
    return PARSER_GEN(T)(EITHER_FN(T), arg);                             \
  }                                                                      \
  END_OF_STATEMENTS

FOREACH(DEFINE_EITHER, TYPESET(1));
