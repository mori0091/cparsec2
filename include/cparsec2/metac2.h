/* -*- coding: utf-8-unix -*- */

#pragma once

#define EXPAND(...) __VA_ARGS__

#define EAT(...)

#define EMPTY()
#define DEFER(x) x EMPTY()
#define DEFER2(x) x DEFER(EMPTY)()

// Find the result of testing whether a macros is enclosed or not
#define CAT_STRUCT(a, b) (a, CAT_STRUCT_ b, PRIMITIVE, b)
#define CAT_STRUCT_(...) EXPANDED, ENCLOSED, (__VA_ARGS__) ) EAT (
// CAT_STRUCT(x,y)     -> (x, CAT_STRUCT_ y, PRIMITIVE, y)
// CAT_STRUCT(x,(y,z)) -> (x, EXPANDED     , ENCLOSED , (y,z))

// Pattern match the result of testing if it is enclose or not
#define CAT_(a, _, f, b) CAT_##f(a, b)
#define CAT_ENCLOSED(a, b) a b
#define CAT_PRIMITIVE(a, b) a##b

// Defer the call to the CAT so that we get the updated parameters first
#define CAT(a, b) CAT0(CAT_STRUCT(a, b))
#define CAT0(...) CAT_ __VA_ARGS__
// CAT(a,b)            -> ab
// CAT(f, (x,y))       -> f (x,y)
// CAT((a,b,c), (d,e)) -> (a,b,c) (d,e)

#define IF(x) CAT_PRIMITIVE(IF, x)
#define IF0(t, f) f
#define IF1(t, f) t
// IF(0)(true, false) -> false
// IF(1)(true, false) -> true

#define MAYBE_STRUCT(x) (CAT(MAYBE_STRUCT_, x), NOTHING)
#define MAYBE_STRUCT_JUST(...) EXPANDED, JUST(__VA_ARGS__) ) EAT (
// MAYBE_STRUCT(JUST(x)) -> (EXPANDED, JUST(x))
// MAYBE_STRUCT(FOO)     -> (MAYBE_STRUCT_FOO, NOTHING)

#define MAYBE(x) MAYBE0(MAYBE_STRUCT(x))
#define MAYBE0(x) MAYBE1 x
#define MAYBE1(_, maybe) maybe
// MAYBE(JUST(x)) -> JUST(x)
// MAYBE(FOO)     -> NOTHING

#define HAS_VALUE(maybe) CAT(HAS_VALUE_, maybe)
#define HAS_VALUE_JUST(...) 1
#define HAS_VALUE_NOTHING 0
// HAS_VALUE(JUST(x)) -> 1
// HAS_VALUE(NOTHING) -> 0

#define EXTRACT_VALUE_OF(maybe) CAT(EXTRACT_VALUE_OF_, maybe)
#define EXTRACT_VALUE_OF_JUST(...) __VA_ARGS__
// EXTRACT_VALUE_OF(JUST(FOO)) -> FOO

#define EXTRACT_OR_DEFAULT(x, ...)                                       \
  IF(HAS_VALUE(MAYBE(x)))(EXTRACT_VALUE_OF(x), __VA_ARGS__)
// EXTRACT_OR_DEFAULT(JUST(FOO), DEFAULT) -> FOO
// EXTRACT_OR_DEFAULT(FOO, DEFAULT)       -> DEFAULT

#define AND(x, y) EXTRACT_OR_DEFAULT(CAT(CAT(AND, x), y), 0)
#define AND11 JUST(1)
// AND(0,0) -> 0
// AND(0,1) -> 0
// AND(1,0) -> 0
// AND(1,1) -> 1

#define NOT(x) EXTRACT_OR_DEFAULT(CAT(NOT, x), 0)
#define NOT0 JUST(1)
// NOT(0) -> 1
// NOT(1) -> 0

#define VARIADIC_SIZE(...)                                               \
  VARIADIC_SIZE_I(__VA_ARGS__, 64, 63, 62, 61, 60, 59, 58, 57, 56, 55,   \
                  54, 53, 52, 51, 50, 49, 48, 47, 46, 45, 44, 43, 42,    \
                  41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29,    \
                  28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16,    \
                  15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, )
#define VARIADIC_SIZE_I(                                                 \
    e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14,     \
    e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27,     \
    e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40,     \
    e41, e42, e43, e44, d45, e46, e47, e48, e49, e50, e51, e52, e53,     \
    e54, e55, e56, e57, e58, e59, e60, e61, e62, e63, size, ...)         \
  size

#define NIL NIL
#define IS_NIL(...)                                                      \
  EXTRACT_OR_DEFAULT(CAT(IS_NIL, VARIADIC_SIZE(__VA_ARGS__)), 0)
#define IS_NIL1 JUST(1)
// IS_NIL()    -> 1
// IS_NIL(a)   -> 1
// IS_NIL(a,b) -> 0
// IS_NIL(,,)  -> 0
// IS_NIL(,)   -> 0
// IS_NIL(,a)  -> 0
// IS_NIL(a,)  -> 0
// IS_NIL((a,b), (x,y), 3) -> 0

#define IS_ENCLOSED_TEST(...) JUST(1)
#define IS_ENCLOSED(x, ...) EXTRACT_OR_DEFAULT(IS_ENCLOSED_TEST x, 0)
// IS_ENCLOSED(a, b)   -> 0
// IS_ENCLOSED((a, b)) -> 1
// IS_ENCLOSED((a), b) -> 1
// IS_ENCLOSED(a, (b)) -> 0

#define IF_ENCLOSED(...) CAT(IF_ENCLOSED, IS_ENCLOSED(__VA_ARGS__))
#define IF_ENCLOSED0(t, ...) __VA_ARGS__
#define IF_ENCLOSED1(t, ...) t
// IF_ENCLOSED(a,b)(true, false)   -> false
// IF_ENCLOSED((a,b))(true, false) -> true

#define ENCLOSE(...) (__VA_ARGS__)
#define DISCLOSE(...)                                                    \
  IF_ENCLOSED(__VA_ARGS__)(DISCLOSE0(__VA_ARGS__), __VA_ARGS__)
#define DISCLOSE0(...) DISCLOSE1 __VA_ARGS__
#define DISCLOSE1(...) __VA_ARGS__
// ENCLOSE(a,b,c)    -> (a,b,c)
// DISCLOSE(a,b,c)   -> a,b,c
// DISCLOSE((a,b,c)) -> a,b,c

#define COMMA() ,
#define SEMICOLON() ;

#define FOLDL(f, ...) FOLDL_EVAL(FOLDL_I(f, __VA_ARGS__, NIL))
#define FOLDL_I(f, x, y, ...)                                            \
  IF(IS_NIL(__VA_ARGS__))                                                \
  (f(x, y), DEFER2(FOLDL_INDIRECT)()(f, f(x, y), __VA_ARGS__))
#define FOLDL_INDIRECT() FOLDL_I
#define FOLDL_EVAL1(...) __VA_ARGS__
#define FOLDL_EVAL2(...) FOLDL_EVAL1(FOLDL_EVAL1(__VA_ARGS__))
#define FOLDL_EVAL3(...) FOLDL_EVAL2(FOLDL_EVAL2(__VA_ARGS__))
#define FOLDL_EVAL4(...) FOLDL_EVAL3(FOLDL_EVAL3(__VA_ARGS__))
#define FOLDL_EVAL5(...) FOLDL_EVAL4(FOLDL_EVAL4(__VA_ARGS__))
#define FOLDL_EVAL6(...) FOLDL_EVAL5(FOLDL_EVAL5(__VA_ARGS__))
#define FOLDL_EVAL7(...) FOLDL_EVAL6(FOLDL_EVAL6(__VA_ARGS__))
#define FOLDL_EVAL8(...) FOLDL_EVAL7(FOLDL_EVAL7(__VA_ARGS__))
#define FOLDL_EVAL(...) FOLDL_EVAL8(__VA_ARGS__)
// FOLDL(f, a, b, c)             -> f(f(a, b), c)
// FOLDL(F, (a,b), (c,d), (e,f)) -> F(F((a,b), (c,d)), (e,f))
// FOLDL(AND, 1, 1, 1, 1)        -> 1
// FOLDL(AND, 1, 1, 0, 1)        -> 0
// #define add(x,y) (x + y)
// FOLDL(add, 1, 2, 3)           -> ((1 + 2) + 3)
// FOLDL(add, 1, 2, 3, 4)        -> (((1 + 2) + 3) + 4)
// FOLDL(add, 1, 2, 3, 4, 5)     -> ((((1 + 2) + 3) + 4) + 5)

#define APPLY(f, ...) APPLY_EVAL(APPLY_I(f, __VA_ARGS__, NIL))
#define APPLY_I(f, x, ...)                                               \
  IF(IS_NIL(__VA_ARGS__))                                                \
  (f(x), f(x) DEFER2(COMMA)() DEFER2(APPLY_INDIRECT)()(f, __VA_ARGS__))
#define APPLY_INDIRECT() APPLY_I
#define APPLY_EVAL1(...) __VA_ARGS__
#define APPLY_EVAL2(...) APPLY_EVAL1(APPLY_EVAL1(__VA_ARGS__))
#define APPLY_EVAL3(...) APPLY_EVAL2(APPLY_EVAL2(__VA_ARGS__))
#define APPLY_EVAL4(...) APPLY_EVAL3(APPLY_EVAL3(__VA_ARGS__))
#define APPLY_EVAL5(...) APPLY_EVAL4(APPLY_EVAL4(__VA_ARGS__))
#define APPLY_EVAL6(...) APPLY_EVAL5(APPLY_EVAL5(__VA_ARGS__))
#define APPLY_EVAL7(...) APPLY_EVAL6(APPLY_EVAL6(__VA_ARGS__))
#define APPLY_EVAL8(...) APPLY_EVAL7(APPLY_EVAL7(__VA_ARGS__))
#define APPLY_EVAL(...) APPLY_EVAL8(__VA_ARGS__)
// APPLY(F, a, b, c)
// -> F(a) , F(b) , F(c)
// APPLY(F, (a,b), (c,d))
// -> F((a,b)) , F((c,d))

#define SEP_BY(sep, f, ...)                                              \
  SEP_BY_EVAL(SEP_BY_I(sep, f, __VA_ARGS__, NIL))
#define SEP_BY_I(sep, f, x, ...)                                         \
  IF(IS_NIL(__VA_ARGS__))                                                \
  (f(x),                                                                 \
   f(x) DEFER2(sep)() DEFER2(SEP_BY_INDIRECT)()(sep, f, __VA_ARGS__))
#define SEP_BY_INDIRECT() SEP_BY_I
#define SEP_BY_EVAL1(...) __VA_ARGS__
#define SEP_BY_EVAL2(...) SEP_BY_EVAL1(SEP_BY_EVAL1(__VA_ARGS__))
#define SEP_BY_EVAL3(...) SEP_BY_EVAL2(SEP_BY_EVAL2(__VA_ARGS__))
#define SEP_BY_EVAL4(...) SEP_BY_EVAL3(SEP_BY_EVAL3(__VA_ARGS__))
#define SEP_BY_EVAL5(...) SEP_BY_EVAL4(SEP_BY_EVAL4(__VA_ARGS__))
#define SEP_BY_EVAL6(...) SEP_BY_EVAL5(SEP_BY_EVAL5(__VA_ARGS__))
#define SEP_BY_EVAL7(...) SEP_BY_EVAL6(SEP_BY_EVAL6(__VA_ARGS__))
#define SEP_BY_EVAL8(...) SEP_BY_EVAL7(SEP_BY_EVAL7(__VA_ARGS__))
#define SEP_BY_EVAL(...) SEP_BY_EVAL8(__VA_ARGS__)
// SEP_BY(SEMICOLON, F, a, b, c)
// -> F(a) ; F(b) ; F(c)
// SEP_BY(SEMICOLON, F, (a,b), (c,d))
// -> F((a,b)) ; F((c,d))

#define FOR_EACH(f, ...) FOR_EACH_EVAL(FOR_EACH_I(f, __VA_ARGS__, NIL))
#define FOR_EACH_I(f, x, ...)                                            \
  IF(IS_NIL(__VA_ARGS__))                                                \
  (f(x), f(x) DEFER2(FOR_EACH_INDIRECT)()(f, __VA_ARGS__))
#define FOR_EACH_INDIRECT() FOR_EACH_I
#define FOR_EACH_EVAL1(...) __VA_ARGS__
#define FOR_EACH_EVAL2(...) FOR_EACH_EVAL1(FOR_EACH_EVAL1(__VA_ARGS__))
#define FOR_EACH_EVAL3(...) FOR_EACH_EVAL2(FOR_EACH_EVAL2(__VA_ARGS__))
#define FOR_EACH_EVAL4(...) FOR_EACH_EVAL3(FOR_EACH_EVAL3(__VA_ARGS__))
#define FOR_EACH_EVAL5(...) FOR_EACH_EVAL4(FOR_EACH_EVAL4(__VA_ARGS__))
#define FOR_EACH_EVAL6(...) FOR_EACH_EVAL5(FOR_EACH_EVAL5(__VA_ARGS__))
#define FOR_EACH_EVAL7(...) FOR_EACH_EVAL6(FOR_EACH_EVAL6(__VA_ARGS__))
#define FOR_EACH_EVAL8(...) FOR_EACH_EVAL7(FOR_EACH_EVAL7(__VA_ARGS__))
#define FOR_EACH_EVAL(...) FOR_EACH_EVAL8(__VA_ARGS__)
// FOR_EACH(F, a, b, c)
// -> F(a) F(b) F(c)
// FOR_EACH(F, (a,b), (c,d))
// -> F((a,b)) F((c,d))

#define GENERIC(expr, f, g, ...)                                         \
  _Generic((expr), GENERIC_SELECTORS(f, g, __VA_ARGS__))
#define GENERIC_SELECTORS(f, g, ...)                                     \
  GENERIC_SELECTORS_EVAL(GENERIC_SELECTORS_I(f, g, __VA_ARGS__, NIL))
#define GENERIC_SELECTORS_I(f, g, x, ...)                                \
  IF(IS_NIL(__VA_ARGS__))                                                \
  (f(x)                                                                  \
   : g(x), f(x)                                                          \
   : g(x) DEFER2(COMMA)()                                                \
       DEFER2(GENERIC_SELECTORS_INDIRECT)()(f, g, __VA_ARGS__))
#define GENERIC_SELECTORS_INDIRECT() GENERIC_SELECTORS_I
#define GENERIC_SELECTORS_EVAL1(...) __VA_ARGS__
#define GENERIC_SELECTORS_EVAL2(...)                                     \
  GENERIC_SELECTORS_EVAL1(GENERIC_SELECTORS_EVAL1(__VA_ARGS__))
#define GENERIC_SELECTORS_EVAL3(...)                                     \
  GENERIC_SELECTORS_EVAL2(GENERIC_SELECTORS_EVAL2(__VA_ARGS__))
#define GENERIC_SELECTORS_EVAL4(...)                                     \
  GENERIC_SELECTORS_EVAL3(GENERIC_SELECTORS_EVAL3(__VA_ARGS__))
#define GENERIC_SELECTORS_EVAL5(...)                                     \
  GENERIC_SELECTORS_EVAL4(GENERIC_SELECTORS_EVAL4(__VA_ARGS__))
#define GENERIC_SELECTORS_EVAL6(...)                                     \
  GENERIC_SELECTORS_EVAL5(GENERIC_SELECTORS_EVAL5(__VA_ARGS__))
#define GENERIC_SELECTORS_EVAL7(...)                                     \
  GENERIC_SELECTORS_EVAL6(GENERIC_SELECTORS_EVAL6(__VA_ARGS__))
#define GENERIC_SELECTORS_EVAL8(...)                                     \
  GENERIC_SELECTORS_EVAL7(GENERIC_SELECTORS_EVAL7(__VA_ARGS__))
#define GENERIC_SELECTORS_EVAL(...) GENERIC_SELECTORS_EVAL8(__VA_ARGS__)
