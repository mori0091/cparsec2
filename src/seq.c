/* -*- coding:utf-8-unix -*- */

#include "cparsec2.h"

#define SEQ_FN(T) CAT(run_seq, T)
#define DEFINE_SEQ(T)                                                    \
  static List(T) SEQ_FN(T)(void* arg, Source src, Ctx* ex) {             \
    PARSER(T)* p = (PARSER(T)*)arg;                                      \
    Buff(T) str = {0};                                                   \
    Ctx ctx;                                                             \
    TRY(&ctx) {                                                          \
      while (*p) {                                                       \
        buff_push(&str, parse(*p, src, &ctx));                           \
        p++;                                                             \
      }                                                                  \
      return buff_finish(&str);                                          \
    }                                                                    \
    else {                                                               \
      mem_free((void*)str.data);                                         \
      /* catch and re-throw exception */                                 \
      cthrow(ex, ctx.msg);                                               \
    }                                                                    \
  }                                                                      \
  PARSER(List(T)) SEQ(T)(void* ps[]) {                                   \
    Buff(Ptr) buf = {0};                                                 \
    while (*ps) {                                                        \
      buff_push(&buf, *ps++);                                            \
    }                                                                    \
    buff_push(&buf, NULL);                                               \
    void* arg = list_begin(buff_finish(&buf));                           \
    return PARSER_GEN(List(T))(SEQ_FN(T), arg);                          \
  }                                                                      \
  _Static_assert(1, "")

DEFINE_P0(SEQ);
