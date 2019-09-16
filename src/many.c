/* -*- coding:utf-8-unix -*- */

#include "cparsec2.h"

#define MANY_FN(T) CAT(run_many, T)
#define DEFINE_MANY(T)                                                   \
  static List(T) MANY_FN(T)(void* arg, Source src, Ctx* ex) {            \
    UNUSED(ex);                                                          \
    PARSER(T) parser = (PARSER(T))arg;                                   \
    Buff(T) str = {0};                                                   \
    off_t offset;                                                        \
    Ctx ctx;                                                             \
    TRY(&ctx) {                                                          \
      for (;;) {                                                         \
        offset = getSourceOffset(src);                                   \
        buff_push(&str, parse(parser, src, &ctx));                       \
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
    return buff_finish(&str);                                            \
  }                                                                      \
  PARSER(List(T)) MANY(T)(PARSER(T) p) {                                 \
    return PARSER_GEN(List(T))(MANY_FN(T), p);                           \
  }                                                                      \
  END_OF_STATEMENTS

FOREACH(DEFINE_MANY, TYPESET(0));
