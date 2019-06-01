/* -*- coding:utf-8-unix -*- */

#include "cparsec2.h"

#define EITHER_FN(T) run_either##T
#define DEFINE_EITHER(T, R)                                              \
  static R EITHER_FN(T)(void* arg, Source src, Ctx* ex) {                \
    PARSER(T)* p = (PARSER(T)*)arg;                                      \
    const char* state = src->p;                                          \
    Ctx ctx;                                                             \
    TRY(&ctx) { return parse(p[0], src, &ctx); }                         \
    else if (state != src->p) {                                          \
      cthrow(ex, ctx.msg);                                               \
    }                                                                    \
    else {                                                               \
      return parse(p[1], src, ex);                                       \
    }                                                                    \
  }                                                                      \
  PARSER(T) EITHER(T)(PARSER(T) p1, PARSER(T) p2) {                      \
    PARSER(T)* arg = mem_malloc(sizeof(PARSER(T)[2]));                   \
    arg[0] = p1;                                                         \
    arg[1] = p2;                                                         \
    return PARSER_GEN(T)(EITHER_FN(T), arg);                             \
  }                                                                      \
  _Static_assert(1, "")

PARSER(Char) EITHER(c)(char c1, char c2) {
  return EITHER(Char)(char1(c1), char1(c2));
}

PARSER(String) EITHER(s)(const char* s1, const char* s2) {
  return EITHER(String)(string1(s1), string1(s2));
}

DEFINE_EITHER(Char, char);
DEFINE_EITHER(String, const char*);
DEFINE_EITHER(Int, int);
DEFINE_EITHER(Token, Token);
