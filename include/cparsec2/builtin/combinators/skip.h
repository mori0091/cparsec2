/* -*- coding:utf-8-unix -*- */
#pragma once

#include "../../alloc.h"
#include "../../exception.h"
#include "../../macros.h"
#include "../../source.h"

#include "../../parser.h"
#include "../../parser_cast.h"

#define SKIP(T) CAT(skip_, T)
#define SKIP_FN(T) run_skip##T

#define DECLARE_SKIP(T) PARSER(None) SKIP(T)(PARSER(T) p)

#define DEFINE_SKIP(T)                                                   \
  static None SKIP_FN(T)(void* arg, Source src, Ctx* ex) {               \
    PARSER(T) p = (PARSER(T))arg;                                        \
    PARSE(T)(p, src, ex);                                                \
    return NONE;                                                         \
  }                                                                      \
  PARSER(None) SKIP(T)(PARSER(T) p) {                                    \
    return PARSER_GEN(None)(SKIP_FN(T), (void*)p);                       \
  }                                                                      \
  END_OF_STATEMENTS

CPARSEC2_C_API_BEGIN
FOREACH(DECLARE_SKIP, TYPESET(1));
CPARSEC2_C_API_END

#ifdef __cplusplus

#define skip(p) cxx_skip(parser_cast(p))
#define DEFINE_CXX_SKIP(T)                                               \
  inline auto cxx_skip(PARSER(T) p) {                                    \
    return SKIP(T)(p);                                                   \
  }                                                                      \
  END_OF_STATEMENTS
FOREACH(DEFINE_CXX_SKIP, TYPESET(1));

#else

#define skip(p)                                                          \
  (GENERIC_P(PARSER_CAST(p), SKIP, TYPESET(1))(PARSER_CAST(p)))

#endif
