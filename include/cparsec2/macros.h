/* -*- coding:utf-8-unix -*- */
#pragma once

#define CAT(x,y) CAT_0(x,y)
#define CAT_0(x,y) x ## y

#define EXPAND(...) __VA_ARGS__

// ** Lists of supporting primitive typenames (except for Char, Ptr) **
// clang-format off
#define GENERIC_SELECTORS(P, F, C)              \
  P(C(String)) : F(C(String)),                  \
  P(C(Int))    : F(C(Int))
#define GENERIC_STATEMENTS(F, C)                \
  F(C(String));                                 \
  F(C(Int))
// clang-format on

// ---- generic macros for containers
// clang-format off
#define GENERIC_METHOD(expr, CLASS, METHOD)           \
  _Generic((expr)                                     \
           , CLASS(Ptr)  : METHOD(Ptr)                \
           , CLASS(Char) : METHOD(Char)               \
           , GENERIC_SELECTORS(CLASS, METHOD, EXPAND) \
           )
// clang-format on

// ---- generic macros for parser-combinators
#define GENERIC_FORALL_P0_SELECTORS(F)                                   \
  PARSER(Char) : F(Char), GENERIC_SELECTORS(PARSER, F, EXPAND)
#define GENERIC_FORALL_P1_SELECTORS(F)                                   \
  GENERIC_FORALL_P0_SELECTORS(F), GENERIC_SELECTORS(PARSER, F, List)
#define GENERIC_FORALL_P0_STATEMENTS(F)                                  \
  F(Char);                                                               \
  GENERIC_STATEMENTS(F, EXPAND)
#define GENERIC_FORALL_P1_STATEMENTS(F)                                  \
  GENERIC_FORALL_P0_STATEMENTS(F);                                       \
  GENERIC_STATEMENTS(F, List)
// ----
#define GENERIC_FORALL_P0(expr, F)                                       \
  _Generic((expr), GENERIC_FORALL_P0_SELECTORS(F))
#define GENERIC_FORALL_P1(expr, F)                                       \
  _Generic((expr), GENERIC_FORALL_P1_SELECTORS(F))
#define DECLARE_FORALL_P0(COMBINATOR)                                    \
  GENERIC_FORALL_P0_STATEMENTS(CAT(DECLARE_, COMBINATOR))
#define DECLARE_FORALL_P1(COMBINATOR)                                    \
  GENERIC_FORALL_P1_STATEMENTS(CAT(DECLARE_, COMBINATOR))
#define DEFINE_FORALL_P0(COMBINATOR)                                     \
  GENERIC_FORALL_P0_STATEMENTS(CAT(DEFINE_, COMBINATOR))
#define DEFINE_FORALL_P1(COMBINATOR)                                     \
  GENERIC_FORALL_P1_STATEMENTS(CAT(DEFINE_, COMBINATOR))
// ----
// clang-format off
#define PARSER_CAST(x)                                  \
  _Generic((x)                                          \
           , char                 : char1               \
           , char*                : string1             \
           , GENERIC_FORALL_P1_SELECTORS(PARSER_ID_FN)  \
           )(x)
// clang-format on
