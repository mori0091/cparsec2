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
    if (!isSourcePosEqual(pos, getSourcePos(src))) {                     \
      cthrow(ex, ctx.msg);                                               \
    }                                                                    \
    mem_free((void*)ctx.msg);                                            \
    ParseError err1 = getParseError(src);                                \
    TRY(&ctx) {                                                          \
      return parse(p[1], src, &ctx);                                     \
    }                                                                    \
    ParseError err2 = getParseError(src);                                \
    setParseError(src, ParseError_merge(err1, err2));                    \
    cthrow(ex, ctx.msg);                                                 \
  }                                                                      \
  PARSER(T) EITHER(T)(PARSER(T) p1, PARSER(T) p2) {                      \
    PARSER(T)* arg = mem_malloc(sizeof(PARSER(T)[2]));                   \
    arg[0] = p1;                                                         \
    arg[1] = p2;                                                         \
    return PARSER_GEN(T)(EITHER_FN(T), arg);                             \
  }                                                                      \
  END_OF_STATEMENTS

FOREACH(DEFINE_EITHER, TYPESET(1));
