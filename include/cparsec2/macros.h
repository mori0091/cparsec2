/* -*- coding:utf-8-unix -*- */
#pragma once

#include "metac.h"

#define CAT(x, y) CAT_0(x, y)
#define CAT_0(x, y) x##y

#define EXPAND(...) __VA_ARGS__

#ifdef __cplusplus
#define END_OF_STATEMENTS static_assert(1, "end of statement")
#else
#define END_OF_STATEMENTS _Static_assert(1, "end of statement")
#endif

#define ELEMENT_TYPESET TYPESET(0), Ptr
#define TYPESET(n) Char, CAT(TYPESET_, n)()
#define TYPESET_0() String, Int
#define TYPESET_1() TYPESET_0(), METAC_APPLY(List, TYPESET_0())

// ---- F(T); ... for each T in varargs
#define FOREACH(F, ...) METAC_SEP_BY(METAC_SEMICOLON, F, __VA_ARGS__)

// ---- _Generic(expr, C(T) : F(T), ...) for each T in varargs
#define GENERIC_METHOD(expr, C, F, ...)                                  \
  METAC_GENERIC(expr, C, F, __VA_ARGS__)
// ---- _Generic(expr, PARSER(T) : F(T), ...) for each T in varargs
#define GENERIC_P(expr, F, ...)                                          \
  METAC_GENERIC(expr, PARSER, F, __VA_ARGS__)

// ---- generic macros for parser-combinators
#define PARSER_CAST(expr)                                                \
  _Generic((expr)                                                        \
           , char  : char1                                               \
           , char* : string1                                             \
           , METAC_GENERIC_SELECTORS(PARSER, PARSER_ID_FN, TYPESET(1))   \
           )(expr)
