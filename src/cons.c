/* -*- coding:utf-8-unix -*- */

#include "cparsec2.h"

#define CONS_FN(T) CAT(run_cons_, T)
#define DEFINE_CONS(T)                                                   \
  static List(T) CONS_FN(T)(void* arg, Source src, Ctx* ex) {            \
    PARSER(T) head = ((void**)arg)[0];                                   \
    PARSER(List(T)) tail = ((void**)arg)[1];                             \
    Buff(T) str = {0};                                                   \
    Ctx ctx;                                                             \
    TRY(&ctx) {                                                          \
      buff_push(&str, parse(head, src, &ctx));                           \
      buff_append(&str, parse(tail, src, &ctx));                         \
      return buff_finish(&str);                                          \
    }                                                                    \
    else {                                                               \
      mem_free(str.data);                                                \
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

FOREACH(DEFINE_CONS, TYPESET(0));
