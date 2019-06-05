/* -*- coding:utf-8-unix -*- */

#include "cparsec2.h"

#define TOKEN_FN(T) run_token##T
#define DEFINE_TOKEN(T, R)                                               \
  static R TOKEN_FN(T)(void* arg, Source src, Ctx* ex) {                 \
    parse(spaces, src, ex);                                              \
    return parse((PARSER(T))arg, src, ex);                               \
  }                                                                      \
  PARSER(T) TOKEN(T)(PARSER(T) p) {                                      \
    return PARSER_GEN(T)(TOKEN_FN(T), p);                                \
  }                                                                      \
  _Static_assert(1, "")

DEFINE_TOKEN(Char, char);
DEFINE_TOKEN(String, const char*);
DEFINE_TOKEN(Int, int);
