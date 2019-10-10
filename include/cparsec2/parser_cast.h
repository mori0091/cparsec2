/* -*- coding:utf-8-unix -*- */
#pragma once

#include "builtin/generators.h"
#include "parser.h"

// ---- generic macros for parser-combinators

#ifdef __cplusplus

#define PARSER_CAST(expr) parser_cast(expr)

template <typename P>
inline P parser_cast(P p) {
  return p;
}
inline auto parser_cast(char c) {
  return char1(c);
}
inline auto parser_cast(const char* s) {
  return string1(s);
}

#else

#define PARSER_CAST(expr)                                                \
  _Generic((expr)                                                 \
           , char  : char1                                        \
           , char* : string1                                      \
           , GENERIC_SELECTORS(PARSER, PARSER_ID_FN, TYPESET(1))  \
           )(expr)

#endif
