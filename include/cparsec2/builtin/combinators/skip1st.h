/* -*- coding:utf-8-unix -*- */
#pragma once

#include "../../alloc.h"
#include "../../exception.h"
#include "../../macros.h"
#include "../../source.h"

#include "../../parser.h"
#include "../../parser_cast.h"

#define SKIP1ST(T) CAT(skip1st_, T)
#define SKIP1ST_FN(T) run_skip1st##T

#define DECLARE_SKIP1ST(T)                                               \
  PARSER(T) SKIP1ST(T)(PARSER(None) p1, PARSER(T) p2)

#define DEFINE_SKIP1ST(T)                                                \
  static RETURN_TYPE(PARSER(T))                                          \
      SKIP1ST_FN(T)(void* arg, Source src, Ctx* ex) {                    \
    void** ps = (void**)arg;                                             \
    PARSE(None)((PARSER(None))ps[0], src, ex);                           \
    return PARSE(T)((PARSER(T))ps[1], src, ex);                          \
  }                                                                      \
  PARSER(T) SKIP1ST(T)(PARSER(None) p1, PARSER(T) p2) {                  \
    void** arg = mem_malloc(sizeof(void*) * 2);                          \
    arg[0] = (void*)p1;                                                  \
    arg[1] = (void*)p2;                                                  \
    return PARSER_GEN(T)(SKIP1ST_FN(T), (void*)arg);                     \
  }                                                                      \
  END_OF_STATEMENTS

CPARSEC2_C_API_BEGIN
FOREACH(DECLARE_SKIP1ST, TYPESET(1));
CPARSEC2_C_API_END

#ifdef __cplusplus

#define skip1st(p1, p2) cxx_skip1st(parser_cast(p1), parser_cast(p2))
#define DEFINE_CXX_SKIP1ST(T)                                            \
  template <typename P>                                                  \
  inline auto cxx_skip1st(P p1, PARSER(T) p2) {                          \
    return SKIP1ST(T)(skip(p1), p2);                                     \
  }                                                                      \
  END_OF_STATEMENTS
FOREACH(DEFINE_CXX_SKIP1ST, TYPESET(1));

#else

#define skip1st(p1, p2)                                                  \
  (GENERIC_P(PARSER_CAST(p2), SKIP1ST, TYPESET(1))(skip(p1),             \
                                                   (PARSER_CAST(p2))))

#endif
