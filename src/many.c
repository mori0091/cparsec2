/* -*- coding:utf-8-unix -*- */

#include "cparsec2.h"

#define MANY_FN(T) CAT(run_many, T)
#define DEFINE_MANY(T)                                                   \
  static List(T) MANY_FN(T)(void* arg, Source src, Ctx* ex) {            \
    UNUSED(ex);                                                          \
    PARSER(T) parser = (PARSER(T))arg;                                   \
    Buff(T) str = {0};                                                   \
    Ctx ctx;                                                             \
    TRY(&ctx) {                                                          \
      for (;;) {                                                         \
        buff_push(&str, parse(parser, src, &ctx));                       \
      }                                                                  \
    }                                                                    \
    else {                                                               \
      /* catch and discard exception */                                  \
      mem_free((void*)ctx.msg);                                          \
    }                                                                    \
    return buff_finish(&str);                                            \
  }                                                                      \
  PARSER(List(T)) MANY(T)(PARSER(T) p) {                                 \
    return PARSER_GEN(List(T))(MANY_FN(T), p);                           \
  }                                                                      \
  _Static_assert(1, "")

DEFINE_MANY(Char);
DEFINE_MANY(String);
DEFINE_MANY(Int);
