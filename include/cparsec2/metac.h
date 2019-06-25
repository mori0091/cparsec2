/* -*- coding: utf-8-unix -*- */
#pragma once

/**
 *\brief CAT(x,y): concatnates x and y
 */
#define METAC_CAT(x, y) METAC_CAT_I(x, y)
#define METAC_CAT_I(x, y) x##y

/**
 * \brief COMPLEMENT(x): 1-bit complement
 * 0 if x was 1, 1 if x was 0.
 */
#define METAC_COMPLEMENT(x) METAC_CAT_I(METAC_COMPLEMENT_, x)
#define METAC_COMPLEMENT_0 1
#define METAC_COMPLEMENT_1 0

/**
 * \brief BITAND(x)(y):  1-bit bitwise AND
 * y if x was 1, 0 if x was 0. */
#define METAC_BITAND(x) METAC_CAT_I(METAC_BITAND_, x)
#define METAC_BITAND_0(y) 0
#define METAC_BITAND_1(y) y

/**
 * \brief BITOR(x)(y):  1-bit bitwise OR
 * 1 if x was 1, y if x was 0.
 */
#define METAC_BITOR(x) METAC_CAT_I(METAC_BITOR_, x)
#define METAC_BITOR_0(y) y
#define METAC_BITOR_1(y) 1

#define METAC_CHECK(...) METAC_CHECK_N(__VA_ARGS__, 0, )
#define METAC_CHECK_N(x, n, ...) n
#define METAC_PROBE(x) x, 1,

#define METAC_IS_PAREN(x) METAC_CHECK(METAC_IS_PAREN_PROBE x)
#define METAC_IS_PAREN_PROBE(...) METAC_PROBE(_)

/**
 * \brief NOT(x): 1 if x was 0, otherwise 0
 */
#define METAC_NOT(x) METAC_CHECK(METAC_CAT_I(METAC_NOT_, x))
#define METAC_NOT_0 METAC_PROBE(_)

/**
 * \brief BOOL(x): 1 if x was not 0, otherwise 0
 */
#define METAC_BOOL(x) METAC_COMPLEMENT(METAC_NOT(x))

/**
 * \brief IIF(x)(t,f): t if x was 1, f if x was 0
 */
#define METAC_IIF(x) METAC_CAT(METAC_IIF_, x)
#define METAC_IIF_0(t, f) f
#define METAC_IIF_1(t, f) t

/**
 * \brief IF(x)(t,f): t if x was not 0, f if x was 0
 */
#define METAC_IF(c) METAC_IIF(METAC_BOOL(c))

/**
 * \brief EAT(...): consumes a subsequent paren
 */
#define METAC_EAT(...)
/**
 * \brief EXPAND(...): expands token(s) in a subsequent paren
 */
#define METAC_EXPAND(...) __VA_ARGS__
/**
 * \brief WHEN(c)(t): expands t when and only when c was not 0
 */
#define METAC_WHEN(c) METAC_IF(c)(METAC_EXPAND, METAC_EAT)

#define METAC_EMPTY()
#define METAC_DEFER(id) id METAC_EMPTY()
#define METAC_DEFER2(id) id METAC_DEFER(METAC_EMPTY)()

/**
 * \brief EVAL(...): expands and expands and expands ...
 */
#define METAC_EVAL1(...) __VA_ARGS__
#define METAC_EVAL2(...) METAC_EVAL1(METAC_EVAL1(__VA_ARGS__))
#define METAC_EVAL3(...) METAC_EVAL2(METAC_EVAL2(__VA_ARGS__))
#define METAC_EVAL4(...) METAC_EVAL3(METAC_EVAL3(__VA_ARGS__))
#define METAC_EVAL5(...) METAC_EVAL4(METAC_EVAL4(__VA_ARGS__))
#define METAC_EVAL6(...) METAC_EVAL5(METAC_EVAL5(__VA_ARGS__))
#define METAC_EVAL7(...) METAC_EVAL6(METAC_EVAL6(__VA_ARGS__))
#define METAC_EVAL8(...) METAC_EVAL7(METAC_EVAL7(__VA_ARGS__))
#define METAC_EVAL(...) METAC_EVAL8(__VA_ARGS__)

/**
 * \brief EQUAL(x,y):
 * true if both of x and y are comparable and if x == y.
 */
#define METAC_EQUAL(x, y) METAC_COMPLEMENT(METAC_NOT_EQUAL(x, y))
/**
 * \brief NOT_EQUAL(x,y):
 * true if either x or y is not comparable or if x != y.
 */
#define METAC_NOT_EQUAL(x, y)                                            \
  METAC_IIF(                                                             \
      METAC_BITAND(METAC_IS_COMPARABLE(x))(METAC_IS_COMPARABLE(y)))      \
  (METAC_COMPARE, 1 METAC_EAT)(x, y)

#define METAC_IS_COMPARABLE(x)                                           \
  METAC_IS_PAREN(METAC_CAT(METAC_COMPARE_, x)(()))
#define METAC_COMPARE(x, y)                                              \
  METAC_IS_PAREN(METAC_COMPARE_##x(METAC_COMPARE_##y)(()))

#define METAC_COMPARE_0(x) x
#define METAC_COMPARE_1(x) x
#define METAC_COMPARE_2(x) x
#define METAC_COMPARE_3(x) x
#define METAC_COMPARE_4(x) x
#define METAC_COMPARE_5(x) x
#define METAC_COMPARE_6(x) x
#define METAC_COMPARE_7(x) x
#define METAC_COMPARE_8(x) x
#define METAC_COMPARE_9(x) x
#define METAC_COMPARE_10(x) x
#define METAC_COMPARE_11(x) x
#define METAC_COMPARE_12(x) x
#define METAC_COMPARE_13(x) x
#define METAC_COMPARE_14(x) x
#define METAC_COMPARE_15(x) x
#define METAC_COMPARE_16(x) x
#define METAC_COMPARE_17(x) x
#define METAC_COMPARE_18(x) x
#define METAC_COMPARE_19(x) x
#define METAC_COMPARE_20(x) x
#define METAC_COMPARE_21(x) x
#define METAC_COMPARE_22(x) x
#define METAC_COMPARE_23(x) x
#define METAC_COMPARE_24(x) x
#define METAC_COMPARE_25(x) x
#define METAC_COMPARE_26(x) x
#define METAC_COMPARE_27(x) x
#define METAC_COMPARE_28(x) x
#define METAC_COMPARE_29(x) x
#define METAC_COMPARE_30(x) x
#define METAC_COMPARE_31(x) x
#define METAC_COMPARE_32(x) x

/**
 * \brief VARIAIC_SIZE(...):
 * How much elements are in the comma separated list? (1..32)
 */
#define METAC_VARIADIC_SIZE(...)                                         \
  METAC_VARIADIC_SIZE_I(__VA_ARGS__, 32, 31, 30, 29, 28, 27, 26, 25, 24, \
                        23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12,  \
                        11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, )
#define METAC_VARIADIC_SIZE_I(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9,    \
                              e10, e11, e12, e13, e14, e15, e16, e17,    \
                              e18, e19, e20, e21, e22, e23, e24, e25,    \
                              e26, e27, e28, e29, e30, e31, size, ...)   \
  size

#define METAC_IS_NIL(...)                                                \
  METAC_EQUAL(METAC_VARIADIC_SIZE(__VA_ARGS__, ), 2)

/**
 * \brief FOLD(f,a,...):
 * Folds ; FOLD(f,a,1,2,3) --> f(f(f(a,1),2),3)
 * ~~~c
 * #define add(x,y) x + y
 * #define SUM(...) METAC_FOLD(add,0,__VA_ARGS__)
 * SUM(1,2,3) --> expands to 0 + 1 + 2 + 3
 * ~~~
 */
#define METAC_FOLD(f, a, ...)                                            \
  METAC_FOLD_EVAL(METAC_FOLD_I(f, a, __VA_ARGS__, ))
#define METAC_FOLD_I(f, a, x, ...)                                       \
  METAC_IF(METAC_IS_NIL(__VA_ARGS__))                                    \
  (f(a, x), METAC_DEFER2(METAC_FOLD_INDIRECT)()(f, f(a, x), __VA_ARGS__))
#define METAC_FOLD_INDIRECT() METAC_FOLD_I
#define METAC_FOLD_EVAL1(...) __VA_ARGS__
#define METAC_FOLD_EVAL2(...)                                            \
  METAC_FOLD_EVAL1(METAC_FOLD_EVAL1(__VA_ARGS__))
#define METAC_FOLD_EVAL3(...)                                            \
  METAC_FOLD_EVAL2(METAC_FOLD_EVAL2(__VA_ARGS__))
#define METAC_FOLD_EVAL4(...)                                            \
  METAC_FOLD_EVAL3(METAC_FOLD_EVAL3(__VA_ARGS__))
#define METAC_FOLD_EVAL5(...)                                            \
  METAC_FOLD_EVAL4(METAC_FOLD_EVAL4(__VA_ARGS__))
#define METAC_FOLD_EVAL6(...)                                            \
  METAC_FOLD_EVAL5(METAC_FOLD_EVAL5(__VA_ARGS__))
#define METAC_FOLD_EVAL7(...)                                            \
  METAC_FOLD_EVAL6(METAC_FOLD_EVAL6(__VA_ARGS__))
#define METAC_FOLD_EVAL8(...)                                            \
  METAC_FOLD_EVAL7(METAC_FOLD_EVAL7(__VA_ARGS__))
#define METAC_FOLD_EVAL(...) METAC_FOLD_EVAL8(__VA_ARGS__)

/**
 * \brief METAC_SEP_BY(sep, f, ...):
 * SEP_BY(SEMICOLON, f, a, b, c)
 * --> f(a); f(b); f(c)
 */
#define METAC_SEP_BY(sep, f, ...)                                        \
  METAC_SEP_BY_EVAL(METAC_SEP_BY_I(sep, f, __VA_ARGS__, ))
#define METAC_SEP_BY_I(sep, f, x, ...)                                   \
  METAC_IF(METAC_IS_NIL(__VA_ARGS__))                                    \
  (f(x), f(x) METAC_DEFER2(sep)()                                        \
             METAC_DEFER2(METAC_SEP_BY_INDIRECT)()(sep, f, __VA_ARGS__))
#define METAC_SEP_BY_INDIRECT() METAC_SEP_BY_I
#define METAC_SEP_BY_EVAL1(...) __VA_ARGS__
#define METAC_SEP_BY_EVAL2(...)                                          \
  METAC_SEP_BY_EVAL1(METAC_SEP_BY_EVAL1(__VA_ARGS__))
#define METAC_SEP_BY_EVAL3(...)                                          \
  METAC_SEP_BY_EVAL2(METAC_SEP_BY_EVAL2(__VA_ARGS__))
#define METAC_SEP_BY_EVAL4(...)                                          \
  METAC_SEP_BY_EVAL3(METAC_SEP_BY_EVAL3(__VA_ARGS__))
#define METAC_SEP_BY_EVAL5(...)                                          \
  METAC_SEP_BY_EVAL4(METAC_SEP_BY_EVAL4(__VA_ARGS__))
#define METAC_SEP_BY_EVAL6(...)                                          \
  METAC_SEP_BY_EVAL5(METAC_SEP_BY_EVAL5(__VA_ARGS__))
#define METAC_SEP_BY_EVAL7(...)                                          \
  METAC_SEP_BY_EVAL6(METAC_SEP_BY_EVAL6(__VA_ARGS__))
#define METAC_SEP_BY_EVAL8(...)                                          \
  METAC_SEP_BY_EVAL7(METAC_SEP_BY_EVAL7(__VA_ARGS__))
#define METAC_SEP_BY_EVAL(...) METAC_SEP_BY_EVAL8(__VA_ARGS__)

#define METAC_COMMA() ,
#define METAC_SEMICOLON() ;

/**
 * \brief METAC_APPLY(f, ...):
 * APPLY(f, a, b, c)
 * --> f(a), f(b), f(c)
 */
// #define METAC_APPLY(f, ...) METAC_SEP_BY(METAC_COMMA, f, __VA_ARGS__)
#define METAC_APPLY(f, ...)                                              \
  METAC_APPLY_EVAL(METAC_APPLY_I(f, __VA_ARGS__, ))
#define METAC_APPLY_I(f, x, ...)                                         \
  METAC_IF(METAC_IS_NIL(__VA_ARGS__))                                    \
  (f(x), f(x) METAC_DEFER2(METAC_COMMA)()                                \
             METAC_DEFER2(METAC_APPLY_INDIRECT)()(f, __VA_ARGS__))
#define METAC_APPLY_INDIRECT() METAC_APPLY_I
#define METAC_APPLY_EVAL1(...) __VA_ARGS__
#define METAC_APPLY_EVAL2(...)                                           \
  METAC_APPLY_EVAL1(METAC_APPLY_EVAL1(__VA_ARGS__))
#define METAC_APPLY_EVAL3(...)                                           \
  METAC_APPLY_EVAL2(METAC_APPLY_EVAL2(__VA_ARGS__))
#define METAC_APPLY_EVAL4(...)                                           \
  METAC_APPLY_EVAL3(METAC_APPLY_EVAL3(__VA_ARGS__))
#define METAC_APPLY_EVAL5(...)                                           \
  METAC_APPLY_EVAL4(METAC_APPLY_EVAL4(__VA_ARGS__))
#define METAC_APPLY_EVAL6(...)                                           \
  METAC_APPLY_EVAL5(METAC_APPLY_EVAL5(__VA_ARGS__))
#define METAC_APPLY_EVAL7(...)                                           \
  METAC_APPLY_EVAL6(METAC_APPLY_EVAL6(__VA_ARGS__))
#define METAC_APPLY_EVAL8(...)                                           \
  METAC_APPLY_EVAL7(METAC_APPLY_EVAL7(__VA_ARGS__))
#define METAC_APPLY_EVAL(...) METAC_APPLY_EVAL8(__VA_ARGS__)

/**
 * \brief METAC_GENERIC(expr, f, g, ...):
 * GENERIC(expr, f, g, a, b, c)
 * --> _Generic((expr), f(a) : g(a), f(b) : g(b), f(c) : g(c))
 */
#define METAC_GENERIC(expr, f, g, ...)                                   \
  _Generic((expr), METAC_GENERIC_SELECTORS(f, g, __VA_ARGS__))
#define METAC_GENERIC_SELECTORS(f, g, ...)                               \
  METAC_GENERIC_SELECTORS_EVAL(                                          \
      METAC_GENERIC_SELECTORS_I(f, g, __VA_ARGS__, ))
#define METAC_GENERIC_SELECTORS_I(f, g, x, ...)                          \
  METAC_IF(METAC_IS_NIL(__VA_ARGS__))                                    \
  (f(x)                                                                  \
   : g(x), f(x)                                                          \
   : g(x) METAC_DEFER2(METAC_COMMA)() METAC_DEFER2(                      \
       METAC_GENERIC_SELECTORS_INDIRECT)()(f, g, __VA_ARGS__))
#define METAC_GENERIC_SELECTORS_INDIRECT() METAC_GENERIC_SELECTORS_I
#define METAC_GENERIC_SELECTORS_EVAL1(...) __VA_ARGS__
#define METAC_GENERIC_SELECTORS_EVAL2(...)                               \
  METAC_GENERIC_SELECTORS_EVAL1(                                         \
      METAC_GENERIC_SELECTORS_EVAL1(__VA_ARGS__))
#define METAC_GENERIC_SELECTORS_EVAL3(...)                               \
  METAC_GENERIC_SELECTORS_EVAL2(                                         \
      METAC_GENERIC_SELECTORS_EVAL2(__VA_ARGS__))
#define METAC_GENERIC_SELECTORS_EVAL4(...)                               \
  METAC_GENERIC_SELECTORS_EVAL3(                                         \
      METAC_GENERIC_SELECTORS_EVAL3(__VA_ARGS__))
#define METAC_GENERIC_SELECTORS_EVAL5(...)                               \
  METAC_GENERIC_SELECTORS_EVAL4(                                         \
      METAC_GENERIC_SELECTORS_EVAL4(__VA_ARGS__))
#define METAC_GENERIC_SELECTORS_EVAL6(...)                               \
  METAC_GENERIC_SELECTORS_EVAL5(                                         \
      METAC_GENERIC_SELECTORS_EVAL5(__VA_ARGS__))
#define METAC_GENERIC_SELECTORS_EVAL7(...)                               \
  METAC_GENERIC_SELECTORS_EVAL6(                                         \
      METAC_GENERIC_SELECTORS_EVAL6(__VA_ARGS__))
#define METAC_GENERIC_SELECTORS_EVAL8(...)                               \
  METAC_GENERIC_SELECTORS_EVAL7(                                         \
      METAC_GENERIC_SELECTORS_EVAL7(__VA_ARGS__))
#define METAC_GENERIC_SELECTORS_EVAL(...)                                \
  METAC_GENERIC_SELECTORS_EVAL8(__VA_ARGS__)
