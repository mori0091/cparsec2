/* -*- coding:utf-8-unix -*- */
#pragma once

#include "../../alloc.h"
#include "../../exception.h"
#include "../../macros.h"
#include "../../source.h"

#include "../../parser.h"
#include "../../parser_cast.h"

#define MANY(T) CAT(many_, T)
#define MANY_FN(T) CAT(run_many, T)

#define DECLARE_MANY(T) PARSER(List(T)) MANY(T)(PARSER(T) p)

#define DEFINE_MANY(T)                                                   \
  static List(T) MANY_FN(T)(void* arg, Source src, Ctx* ex) {            \
    UNUSED(ex);                                                          \
    PARSER(T) parser = (PARSER(T))arg;                                   \
    Buff(T) buf = {0};                                                   \
    volatile intmax_t offset;                                            \
    Ctx ctx;                                                             \
    TRY(&ctx) {                                                          \
      for (;;) {                                                         \
        offset = getSourceOffset(src);                                   \
        BUFF_PUSH(T)(&buf, PARSE(T)(parser, src, &ctx));                 \
        if (offset == getSourceOffset(src)) {                            \
          cthrow(ex, error("combinator 'many' is applied to a parser "   \
                           "that accepts an empty input."));             \
        }                                                                \
      }                                                                  \
    }                                                                    \
    if (offset != getSourceOffset(src)) {                                \
      cthrow(ex, ctx.msg);                                               \
    }                                                                    \
    mem_free((void*)ctx.msg);                                            \
    return BUFF_FINISH(T)(&buf);                                         \
  }                                                                      \
  PARSER(List(T)) MANY(T)(PARSER(T) p) {                                 \
    return PARSER_GEN(List(T))(MANY_FN(T), p);                           \
  }                                                                      \
  END_OF_STATEMENTS

CPARSEC2_C_API_BEGIN
FOREACH(DECLARE_MANY, TYPESET(0));
CPARSEC2_C_API_END

#ifdef __cplusplus

#define many(p) cxx_many(parser_cast(p))
#define DEFINE_CXX_MANY(T)                                               \
  inline auto cxx_many(PARSER(T) p) {                                    \
    return MANY(T)(p);                                                   \
  }                                                                      \
  END_OF_STATEMENTS
FOREACH(DEFINE_CXX_MANY, TYPESET(0));

#else

#define many(p)                                                          \
  (GENERIC_P(PARSER_CAST(p), MANY, TYPESET(0))(PARSER_CAST(p)))

#endif
