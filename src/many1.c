/* -*- coding:utf-8-unix -*- */

#include "cparsec2.h"

#define MANY1_FN(T) CAT(run_many1, T)
#define DEFINE_MANY1(T)                                                  \
  static List(T) MANY1_FN(T)(void* arg, Source src, Ctx* ex) {           \
    PARSER(T) parser = (PARSER(T))arg;                                   \
    Ctx ctx;                                                             \
    Buff(T) str = {0};                                                   \
    TRY(&ctx) {                                                          \
      buff_push(&str, parse(parser, src, &ctx));                         \
      buff_append(&str, parse(many(parser), src, &ctx));                 \
      return buff_finish(&str);                                          \
    }                                                                    \
    else {                                                               \
      mem_free((void*)str.data);                                         \
      /* catch and re-throw exception */                                 \
      cthrow(ex, ctx.msg);                                               \
    }                                                                    \
  }                                                                      \
  PARSER(List(T)) MANY1(T)(PARSER(T) p) {                                \
    return PARSER_GEN(List(T))(MANY1_FN(T), p);                          \
  }                                                                      \
  _Static_assert(1, "")

DEFINE_MANY1(Char);
DEFINE_MANY1(String);
DEFINE_MANY1(Int);
