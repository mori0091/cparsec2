/* -*- coding:utf-8-unix -*- */
#pragma once

#include "../../alloc.h"
#include "../../exception.h"
#include "../../macros.h"
#include "../../source.h"

#include "../../parser.h"
#include "../../parser_cast.h"

#define CONS(T) CAT(cons_, T)
#define CONS_FN(T) CAT(run_cons_, T)

#define DECLARE_CONS(T)                                                  \
  PARSER(List(T)) CONS(T)(PARSER(T) p, PARSER(List(T)) ps)

#define DEFINE_CONS(T)                                                   \
  static List(T) CONS_FN(T)(void* arg, Source src, Ctx* ex) {            \
    PARSER(T) head = ((void**)arg)[0];                                   \
    PARSER(List(T)) tail = ((void**)arg)[1];                             \
    Buff(T) buf = {0};                                                   \
    Ctx ctx;                                                             \
    TRY(&ctx) {                                                          \
      BUFF_PUSH(T)(&buf, PARSE(T)(head, src, &ctx));                     \
      BUFF_APPEND(T)(&buf, PARSE(List(T))(tail, src, &ctx));             \
      return BUFF_FINISH(T)(&buf);                                       \
    }                                                                    \
    else {                                                               \
      mem_free(buf.data);                                                \
      cthrow(ex, ctx.msg);                                               \
    }                                                                    \
  }                                                                      \
  PARSER(List(T)) CONS(T)(PARSER(T) p, PARSER(List(T)) ps) {             \
    void** arg = mem_malloc(sizeof(void*) * 2);                          \
    arg[0] = p;                                                          \
    arg[1] = ps;                                                         \
    return PARSER_GEN(List(T))(CONS_FN(T), (void*)arg);                  \
  }                                                                      \
  END_OF_STATEMENTS

CPARSEC2_C_API_BEGIN
FOREACH(DECLARE_CONS, TYPESET(0));
CPARSEC2_C_API_END

#ifdef __cplusplus

#define cons(p, ps) cxx_cons(parser_cast(p), parser_cast(ps))
#define DEFINE_CXX_CONS(T)                                               \
  inline auto cxx_cons(PARSER(T) p, PARSER(List(T)) ps) {                \
    return CONS(T)(p, ps);                                               \
  }                                                                      \
  END_OF_STATEMENTS
FOREACH(DEFINE_CXX_CONS, TYPESET(0));

#else

#define cons(p, ps)                                                      \
  (GENERIC_P(PARSER_CAST(p), CONS, TYPESET(0))(PARSER_CAST(p),           \
                                               PARSER_CAST(ps)))

#endif
