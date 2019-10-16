/* -*- coding:utf-8-unix -*- */
#pragma once

#include "macros.h"

#define ELEMENT_TYPESET TYPESET(0), Ptr
#define TYPESET(n) Char, CAT(TYPESET_, n)()
#define TYPESET_0()                                                      \
  SQUASH(String, Int, None, Node, CPARSEC2_USER_TYPESET_I())
#define TYPESET_1() TYPESET_0(), APPLY(List, TYPESET_0())

#ifdef CPARSEC2_USER_TYPESET

#define CPARSEC2_USER_TYPESET_I() CPARSEC2_USER_TYPESET
#define CPARSEC2_DEFINE_USER_TYPESET()                                   \
  FOREACH(CPARSEC2_DEFINE_CONTAINER, SQUASH(CPARSEC2_USER_TYPESET));     \
  FOREACH(CPARSEC2_DEFINE_PARSER_CLASS, SQUASH(CPARSEC2_USER_TYPESET));  \
  FOREACH(CPARSEC2_DEFINE_COMBINATORS, SQUASH(CPARSEC2_USER_TYPESET))

#else

#define CPARSEC2_USER_TYPESET_I()
#define CPARSEC2_DEFINE_USER_TYPESET()                                   \
  static_assert(0, "CPARSEC2_USER_TYPESET is not defined.")

#endif

#define CPARSEC2_DEFINE_CONTAINER(T)                                     \
  CPARSEC2_C_API_BEGIN                                                   \
  DEFINE_LIST(T);                                                        \
  DEFINE_BUFF(T);                                                        \
  CPARSEC2_C_API_END                                                     \
  END_OF_STATEMENTS

#define CPARSEC2_DEFINE_PARSER_CLASS(T)                                  \
  CPARSEC2_C_API_BEGIN                                                   \
  DEFINE_PARSER(T);                                                      \
  DEFINE_RUNPARSER(T);                                                   \
  DEFINE_SHOW_LIST(T);                                                   \
  DEFINE_PARSER(List(T));                                                \
  DEFINE_RUNPARSER(List(T));                                             \
  CPARSEC2_C_API_END                                                     \
  END_OF_STATEMENTS

#define CPARSEC2_DEFINE_COMBINATORS(T)                                   \
  CPARSEC2_C_API_BEGIN                                                   \
  DEFINE_EXPECTS(T);                                                     \
  DEFINE_MANY(T);                                                        \
  DEFINE_MANY1(T);                                                       \
  DEFINE_CONS(T);                                                        \
  DEFINE_SEQ(T);                                                         \
  DEFINE_SKIP(T);                                                        \
  DEFINE_SKIP1ST(T);                                                     \
  DEFINE_SKIP2ND(T);                                                     \
  DEFINE_EITHER(T);                                                      \
  DEFINE_TRYP(T);                                                        \
  DEFINE_TOKEN(T);                                                       \
  CPARSEC2_C_API_END                                                     \
  END_OF_STATEMENTS
