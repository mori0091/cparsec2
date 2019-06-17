/* -*- coding:utf-8-unix -*- */

#include "cparsec2.h"

#define TRYP_FN(T) run_tryp##T
#define DEFINE_TRYP(T)                                                   \
  static RETURN_TYPE(PARSER(T))                                          \
      TRYP_FN(T)(void* arg, Source src, Ctx* ex) {                       \
    PARSER(T) p = (PARSER(T))arg;                                        \
    const char* state = src->p;                                          \
    Ctx ctx;                                                             \
    TRY(&ctx) {                                                          \
      return parse(p, src, &ctx);                                        \
    }                                                                    \
    else {                                                               \
      src->p = state;                                                    \
      cthrow(ex, ctx.msg);                                               \
    }                                                                    \
  }                                                                      \
  PARSER(T) TRYP(T)(PARSER(T) p) {                                       \
    return PARSER_GEN(T)(TRYP_FN(T), (void*)p);                          \
  }                                                                      \
  _Static_assert(1, "")

DEFINE_P1(TRYP);
