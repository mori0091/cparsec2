/* -*- coding:utf-8-unix -*- */
#pragma once

#include "../../alloc.h"
#include "../../exception.h"
#include "../../macros.h"
#include "../../source.h"

#include "../../parser.h"
#include "../../parser_cast.h"

#define EITHER(T) CAT(either_, T)
#define EITHER_FN(T) run_either##T

#define DECLARE_EITHER(T) PARSER(T) EITHER(T)(PARSER(T) p1, PARSER(T) p2)

#define DEFINE_EITHER(T)                                                 \
  static RETURN_TYPE(PARSER(T))                                          \
      EITHER_FN(T)(void* arg, Source src, Ctx* ex) {                     \
    PARSER(T)* p = (PARSER(T)*)arg;                                      \
    intmax_t pos = getSourceOffset(src);                                 \
    Ctx ctx;                                                             \
    TRY(&ctx) {                                                          \
      return PARSE(T)(p[0], src, &ctx);                                  \
    }                                                                    \
    if (pos != getSourceOffset(src)) {                                   \
      cthrow(ex, ctx.msg);                                               \
    }                                                                    \
    mem_free((void*)ctx.msg);                                            \
    ParseError err1 = getParseError(src);                                \
    TRY(&ctx) {                                                          \
      return PARSE(T)(p[1], src, &ctx);                                  \
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

CPARSEC2_C_API_BEGIN
FOREACH(DECLARE_EITHER, TYPESET(1));
CPARSEC2_C_API_END

#ifdef __cplusplus

#define either(p1, p2) cxx_either(parser_cast(p1), parser_cast(p2))
#define DEFINE_CXX_EITHER(T)                                             \
  inline auto cxx_either(PARSER(T) p1, PARSER(T) p2) {                   \
    return EITHER(T)(p1, p2);                                            \
  }                                                                      \
  END_OF_STATEMENTS
FOREACH(DEFINE_CXX_EITHER, TYPESET(1));

#else

#define either(p1, p2)                                                   \
  (GENERIC_P(PARSER_CAST(p1), EITHER, TYPESET(1))(PARSER_CAST(p1),       \
                                                  PARSER_CAST(p2)))

#endif
