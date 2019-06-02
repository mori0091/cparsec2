/* -*- coding:utf-8-unix -*- */

#include "cparsec2.h"

#define TRYP_FN(T) run_tryp##T
#define DEFINE_TRYP(T, R)                                                \
  static R TRYP_FN(T)(void* arg, Source src, Ctx* ex) {                  \
    PARSER(T) p = (PARSER(T))arg;                                        \
    const char* state = src->p;                                          \
    Ctx ctx;                                                             \
    TRY(&ctx) { return parse(p, src, &ctx); }                            \
    else {                                                               \
      src->p = state;                                                    \
      cthrow(ex, ctx.msg);                                               \
    }                                                                    \
  }                                                                      \
  PARSER(T) TRYP(T)(PARSER(T) p) {                                       \
    return PARSER_GEN(T)(TRYP_FN(T), (void*)p);                          \
  }                                                                      \
  _Static_assert(1, "")

PARSER(Char) TRYP(c)(char c) {
  return TRYP(Char)(char1(c));
}

PARSER(String) TRYP(s)(const char* s) {
  return TRYP(String)(string1(s));
}

DEFINE_TRYP(Char, char);
DEFINE_TRYP(String, const char*);
DEFINE_TRYP(Int, int);
DEFINE_TRYP(Token, Token);
