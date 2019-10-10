/* -*- coding:utf-8-unix -*- */
#pragma once

#include "../../alloc.h"
#include "../../exception.h"
#include "../../macros.h"
#include "../../source.h"

#include "../../parser.h"
#include "../../parser_cast.h"

#include <stdint.h>
#include <string.h>

#define EXPECTS(T) CAT(expects_, T)
#define EXPECTS_FN(T) CAT(run_expects, T)

#define DECLARE_EXPECTS(T)                                               \
  PARSER(T) EXPECTS(T)(const char* desc, PARSER(T) p)

#define DEFINE_EXPECTS(T)                                                \
  static RETURN_TYPE(PARSER(T))                                          \
      EXPECTS_FN(T)(void* arg, Source src, Ctx* ex) {                    \
    void** ps = (void**)arg;                                             \
    const char* desc = ps[0];                                            \
    PARSER(T) p = ps[1];                                                 \
    intmax_t pos = getSourceOffset(src);                                 \
    Ctx ctx;                                                             \
    TRY(&ctx) {                                                          \
      return PARSE(T)(p, src, &ctx);                                     \
    }                                                                    \
    if (pos != getSourceOffset(src)) {                                   \
      cthrow(ex, ctx.msg);                                               \
    }                                                                    \
    ErrMsg m = {Expect, desc};                                           \
    parseError(src, m);                                                  \
    const char* bw = strstr(ctx.msg, "but was");                         \
    if (bw) {                                                            \
      const char* msg = error("expects %s %s", desc, bw);                \
      mem_free((void*)ctx.msg);                                          \
      cthrow(ex, msg);                                                   \
    } else {                                                             \
      cthrow(ex, ctx.msg);                                               \
    }                                                                    \
  }                                                                      \
  PARSER(T) EXPECTS(T)(const char* desc, PARSER(T) p) {                  \
    void** ps = mem_malloc(sizeof(void*) * 2);                           \
    ps[0] = (void*)desc;                                                 \
    ps[1] = (void*)p;                                                    \
    return PARSER_GEN(T)(EXPECTS_FN(T), ps);                             \
  }                                                                      \
  END_OF_STATEMENTS

CPARSEC2_C_API_BEGIN
FOREACH(DECLARE_EXPECTS, TYPESET(1));
CPARSEC2_C_API_END

#ifdef __cplusplus

#define expects(desc, p) cxx_expects((desc), parser_cast(p))
#define DEFINE_CXX_EXPECTS(T)                                            \
  inline auto cxx_expects(const char* desc, PARSER(T) p) {               \
    return EXPECTS(T)(desc, p);                                          \
  }                                                                      \
  END_OF_STATEMENTS
FOREACH(DEFINE_CXX_EXPECTS, TYPESET(1));

#else

#define expects(desc, p)                                                 \
  (GENERIC_P(PARSER_CAST(p), EXPECTS, TYPESET(1))((desc), PARSER_CAST(p)))

#endif
