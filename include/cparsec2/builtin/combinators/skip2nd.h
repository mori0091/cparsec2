/* -*- coding:utf-8-unix -*- */
#pragma once

#include "../../alloc.h"
#include "../../exception.h"
#include "../../macros.h"
#include "../../source.h"

#include "../../parser.h"
#include "../../parser_cast.h"

#define SKIP2ND(T) CAT(skip2nd_, T)
#define SKIP2ND_FN(T) run_skip2nd##T

#define DECLARE_SKIP2ND(T)                                               \
  PARSER(T) SKIP2ND(T)(PARSER(T) p1, PARSER(None) p2)

#define DEFINE_SKIP2ND(T)                                                \
  static RETURN_TYPE(PARSER(T))                                          \
      SKIP2ND_FN(T)(void* arg, Source src, Ctx* ex) {                    \
    void** ps = (void**)arg;                                             \
    RETURN_TYPE(PARSER(T)) ret = PARSE(T)((PARSER(T))ps[0], src, ex);    \
    PARSE(None)((PARSER(None))ps[1], src, ex);                           \
    return ret;                                                          \
  }                                                                      \
  PARSER(T) SKIP2ND(T)(PARSER(T) p1, PARSER(None) p2) {                  \
    void** arg = mem_malloc(sizeof(void*) * 2);                          \
    arg[0] = (void*)p1;                                                  \
    arg[1] = (void*)p2;                                                  \
    return PARSER_GEN(T)(SKIP2ND_FN(T), (void*)arg);                     \
  }                                                                      \
  END_OF_STATEMENTS

CPARSEC2_C_API_BEGIN
FOREACH(DECLARE_SKIP2ND, TYPESET(1));
CPARSEC2_C_API_END

#ifdef __cplusplus

#define skip2nd(p1, p2) cxx_skip2nd(parser_cast(p1), parser_cast(p2))
#define DEFINE_CXX_SKIP2ND(T)                                            \
  template <typename P>                                                  \
  inline auto cxx_skip2nd(PARSER(T) p1, P p2) {                          \
    return SKIP2ND(T)(p1, skip(p2));                                     \
  }                                                                      \
  END_OF_STATEMENTS
FOREACH(DEFINE_CXX_SKIP2ND, TYPESET(1));

#else

#define skip2nd(p1, p2)                                                  \
  (GENERIC_P(PARSER_CAST(p1), SKIP2ND, TYPESET(1))((PARSER_CAST(p1)),    \
                                                   skip(p2)))

#endif
