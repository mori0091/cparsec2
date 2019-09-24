/* -*- coding:utf-8-unix -*- */

#include "cparsec2.h"

#define TRYP_FN(T) run_tryp##T
#define DEFINE_TRYP(T)                                                   \
  static RETURN_TYPE(PARSER(T))                                          \
      TRYP_FN(T)(void* arg, Source src, Ctx* ex) {                       \
    PARSER(T) p = (PARSER(T))arg;                                        \
    intmax_t offset = getSourceOffset(src);                              \
    SourcePos pos = getSourcePos(src);                                   \
    Ctx ctx;                                                             \
    TRY(&ctx) {                                                          \
      return parse(p, src, &ctx);                                        \
    }                                                                    \
    else {                                                               \
      setSourceOffset(src, offset);                                      \
      setSourcePos(src, pos);                                            \
      cthrow(ex, ctx.msg);                                               \
    }                                                                    \
  }                                                                      \
  PARSER(T) TRYP(T)(PARSER(T) p) {                                       \
    return PARSER_GEN(T)(TRYP_FN(T), (void*)p);                          \
  }                                                                      \
  END_OF_STATEMENTS

FOREACH(DEFINE_TRYP, TYPESET(1));
