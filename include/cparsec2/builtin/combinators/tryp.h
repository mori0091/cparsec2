/* -*- coding:utf-8-unix -*- */
#pragma once

#include "../../alloc.h"
#include "../../exception.h"
#include "../../macros.h"
#include "../../source.h"

#include "../../parser.h"
#include "../../parser_cast.h"

#define TRYP(T) CAT(tryp_, T)
#define TRYP_FN(T) run_tryp##T

#define DECLARE_TRYP(T) PARSER(T) TRYP(T)(PARSER(T) p)

#define DEFINE_TRYP(T)                                                   \
  static RETURN_TYPE(PARSER(T))                                          \
      TRYP_FN(T)(void* arg, Source src, Ctx* ex) {                       \
    PARSER(T) p = (PARSER(T))arg;                                        \
    intmax_t offset = getSourceOffset(src);                              \
    SourcePos pos = getSourcePos(src);                                   \
    Ctx ctx;                                                             \
    TRY(&ctx) {                                                          \
      return PARSE(T)(p, src, &ctx);                                     \
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

CPARSEC2_C_API_BEGIN
FOREACH(DECLARE_TRYP, TYPESET(1));
CPARSEC2_C_API_END

#ifdef __cplusplus

#define tryp(p) cxx_tryp(parser_cast(p))
#define DEFINE_CXX_TRYP(T)                                               \
  inline auto cxx_tryp(PARSER(T) p) {                                    \
    return TRYP(T)(p);                                                   \
  }                                                                      \
  END_OF_STATEMENTS
FOREACH(DEFINE_CXX_TRYP, TYPESET(1));

#else

#define tryp(p)                                                          \
  (GENERIC_P(PARSER_CAST(p), TRYP, TYPESET(1))(PARSER_CAST(p)))

#endif
