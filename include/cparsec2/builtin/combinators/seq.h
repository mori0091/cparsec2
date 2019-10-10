/* -*- coding:utf-8-unix -*- */
#pragma once

#include "../../alloc.h"
#include "../../exception.h"
#include "../../macros.h"
#include "../../source.h"

#include "../../parser.h"
#include "../../parser_cast.h"

#define SEQ(T) CAT(seq_, T)
#define SEQ_FN(T) CAT(run_seq, T)

#define DECLARE_SEQ(T) PARSER(List(T)) SEQ(T)(void* ps[])

#define DEFINE_SEQ(T)                                                    \
  static List(T) SEQ_FN(T)(void* arg, Source src, Ctx* ex) {             \
    Buff(T) buf = {0};                                                   \
    Ctx ctx;                                                             \
    TRY(&ctx) {                                                          \
      PARSER(T)* p = (PARSER(T)*)arg;                                    \
      while (*p) {                                                       \
        BUFF_PUSH(T)(&buf, PARSE(T)(*p, src, &ctx));                     \
        p++;                                                             \
      }                                                                  \
      return BUFF_FINISH(T)(&buf);                                       \
    }                                                                    \
    else {                                                               \
      mem_free((void*)buf.data);                                         \
      /* catch and re-throw exception */                                 \
      cthrow(ex, ctx.msg);                                               \
    }                                                                    \
  }                                                                      \
  PARSER(List(T)) SEQ(T)(void* ps[]) {                                   \
    Buff(Ptr) buf = {0};                                                 \
    while (*ps) {                                                        \
      BUFF_PUSH(Ptr)(&buf, *ps++);                                       \
    }                                                                    \
    BUFF_PUSH(Ptr)(&buf, NULL);                                          \
    void* arg = LIST_BEGIN(Ptr)(BUFF_FINISH(Ptr)(&buf));                 \
    return PARSER_GEN(List(T))(SEQ_FN(T), arg);                          \
  }                                                                      \
  END_OF_STATEMENTS

CPARSEC2_C_API_BEGIN
FOREACH(DECLARE_SEQ, TYPESET(0));
CPARSEC2_C_API_END

#ifdef __cplusplus

#define seq(...) cxx_seq(__VA_ARGS__)
#define DEFINE_CXX_SEQ(T)                                                \
  template <typename... P>                                               \
  inline auto cxx_seq(PARSER(T) p, P... args) {                          \
    PARSER(T) ps[] = {p, args..., NULL};                                 \
    return SEQ(T)((void**)ps);                                           \
  }                                                                      \
  END_OF_STATEMENTS
FOREACH(DEFINE_CXX_SEQ, TYPESET(0));

#else

#define seq(...) SEQ_0(__VA_ARGS__, NULL)
#define SEQ_0(p, ...)                                                    \
  (GENERIC_P(p, SEQ, TYPESET(0))((void*[]){p, __VA_ARGS__}))

#endif
