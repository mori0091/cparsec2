/* -*- coding:utf-8-unix -*- */
#pragma once

#include "metac2.h"

#ifdef __cplusplus
#define END_OF_STATEMENTS static_assert(1, "end of statement")
#else
#define END_OF_STATEMENTS _Static_assert(1, "end of statement")
#endif

#define UNUSED(v) ((void)v)

#define ELEMENT_TYPESET TYPESET(0), Ptr
#define TYPESET(n) Char, CAT(TYPESET_, n)()
#define TYPESET_0() String, Int, None, Node
#define TYPESET_1() TYPESET_0(), APPLY(List, TYPESET_0())

// ---- F(T); ... for each T in varargs
#define FOREACH(F, ...) SEP_BY(SEMICOLON, F, __VA_ARGS__)

// ---- _Generic(expr, C(T) : F(T), ...) for each T in varargs
#define GENERIC_METHOD(expr, C, F, ...) GENERIC(expr, C, F, __VA_ARGS__)
// ---- _Generic(expr, PARSER(T) : F(T), ...) for each T in varargs
#define GENERIC_P(expr, F, ...) GENERIC(expr, PARSER, F, __VA_ARGS__)

// ---- generic macros for parser-combinators
#define PARSER_CAST(expr)                                                \
  _Generic((expr)                                                        \
           , char  : char1                                               \
           , char* : string1                                             \
           , GENERIC_SELECTORS(PARSER, PARSER_ID_FN, TYPESET(1))         \
           )(expr)
