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

#define IS_ENCLOSED(...) IS_ENCLOSED1(__VA_ARGS__, )
#define IS_ENCLOSED1(x, ...) EXTRACT_OR_DEFAULT(IS_ENCLOSED_TEST x, 0)
#define IS_ENCLOSED_TEST(...) JUST(1)
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

#define IS_NULL(x) EXTRACT_OR_DEFAULT(x JUST(1), 0)
// IS_NULL()      -> 1
// IS_NULL(())    -> 0
// IS_NULL(x)     -> 0
// IS_NULL((a,b)) -> 0
// IS_NULL(1)     -> 0

#define FST(x, y) x
#define SND(x, y) y

#define HEAD(...) HEAD1(__VA_ARGS__, )
#define HEAD1(x, ...) x
#define TAIL(...) IF(IS_NIL(__VA_ARGS__))(, TAIL1(__VA_ARGS__))
#define TAIL1(x, ...) __VA_ARGS__

#define COMMA() ,
#define SEMICOLON() ;

#define TO_REV_LIST(...)                                                 \
  TO_REV_LIST_EVAL(TO_REV_LIST_I(NIL, __VA_ARGS__, ))
#define TO_REV_LIST_I(xs, x, ...)                                        \
  IF(IS_NIL(__VA_ARGS__))                                                \
  ((x, xs), DEFER2(TO_REV_LIST_INDIRECT)()((x, xs), __VA_ARGS__))
#define TO_REV_LIST_INDIRECT() TO_REV_LIST_I
#define TO_REV_LIST_EVAL1(...) __VA_ARGS__
#define TO_REV_LIST_EVAL2(...)                                           \
  TO_REV_LIST_EVAL1(TO_REV_LIST_EVAL1(__VA_ARGS__))
#define TO_REV_LIST_EVAL3(...)                                           \
  TO_REV_LIST_EVAL2(TO_REV_LIST_EVAL2(__VA_ARGS__))
#define TO_REV_LIST_EVAL4(...)                                           \
  TO_REV_LIST_EVAL3(TO_REV_LIST_EVAL3(__VA_ARGS__))
#define TO_REV_LIST_EVAL5(...)                                           \
  TO_REV_LIST_EVAL4(TO_REV_LIST_EVAL4(__VA_ARGS__))
#define TO_REV_LIST_EVAL6(...)                                           \
  TO_REV_LIST_EVAL5(TO_REV_LIST_EVAL5(__VA_ARGS__))
#define TO_REV_LIST_EVAL7(...)                                           \
  TO_REV_LIST_EVAL6(TO_REV_LIST_EVAL6(__VA_ARGS__))
#define TO_REV_LIST_EVAL8(...)                                           \
  TO_REV_LIST_EVAL7(TO_REV_LIST_EVAL7(__VA_ARGS__))
#define TO_REV_LIST_EVAL(...) TO_REV_LIST_EVAL8(__VA_ARGS__)
// TO_REV_LIST(1,2,3,4) -> (4, (3, (2, (1, NIL))))

#define UNLIST(xs) UNLIST_EVAL(UNLIST_I xs)
#define UNLIST_I(x, xs)                                                  \
  x IF_ENCLOSED(xs)(DEFER2(COMMA)() DEFER2(UNLIST_INDIRECT)() xs, )
#define UNLIST_INDIRECT() UNLIST_I
#define UNLIST_EVAL1(...) __VA_ARGS__
#define UNLIST_EVAL2(...) UNLIST_EVAL1(UNLIST_EVAL1(__VA_ARGS__))
#define UNLIST_EVAL3(...) UNLIST_EVAL2(UNLIST_EVAL2(__VA_ARGS__))
#define UNLIST_EVAL4(...) UNLIST_EVAL3(UNLIST_EVAL3(__VA_ARGS__))
#define UNLIST_EVAL5(...) UNLIST_EVAL4(UNLIST_EVAL4(__VA_ARGS__))
#define UNLIST_EVAL6(...) UNLIST_EVAL5(UNLIST_EVAL5(__VA_ARGS__))
#define UNLIST_EVAL7(...) UNLIST_EVAL6(UNLIST_EVAL6(__VA_ARGS__))
#define UNLIST_EVAL8(...) UNLIST_EVAL7(UNLIST_EVAL7(__VA_ARGS__))
#define UNLIST_EVAL(...) UNLIST_EVAL8(__VA_ARGS__)
// UNLIST((1,(2,(3,NIL))))       -> 1,2,3
// UNLIST((1,(2,(3,(4,(5,?)))))) -> 1,2,3,4,5
// UNLIST(NIL)                   ->
// UNLIST(())                    -> error
// UNLIST((1))                   -> error

#define REVERSE(...) UNLIST(TO_REV_LIST(__VA_ARGS__))
// REVERSE(1)     -> 1
// REVERSE(1,2)   -> 2, 1
// REVERSE(1,2,3) -> 3, 2, 1

#define FOLDR(f, ...)                                                    \
  FOLDR_EVAL(DEFER(FOLDR_I)(f, REVERSE(__VA_ARGS__), ))
#define FOLDR_I(f, y, x, ...)                                            \
  IF(IS_NIL(__VA_ARGS__))                                                \
  (f(x, y), DEFER2(FOLDR_INDIRECT)()(f, f(x, y), __VA_ARGS__))
#define FOLDR_INDIRECT() FOLDR_I
#define FOLDR_EVAL1(...) __VA_ARGS__
#define FOLDR_EVAL2(...) FOLDR_EVAL1(FOLDR_EVAL1(__VA_ARGS__))
#define FOLDR_EVAL3(...) FOLDR_EVAL2(FOLDR_EVAL2(__VA_ARGS__))
#define FOLDR_EVAL4(...) FOLDR_EVAL3(FOLDR_EVAL3(__VA_ARGS__))
#define FOLDR_EVAL5(...) FOLDR_EVAL4(FOLDR_EVAL4(__VA_ARGS__))
#define FOLDR_EVAL6(...) FOLDR_EVAL5(FOLDR_EVAL5(__VA_ARGS__))
#define FOLDR_EVAL7(...) FOLDR_EVAL6(FOLDR_EVAL6(__VA_ARGS__))
#define FOLDR_EVAL8(...) FOLDR_EVAL7(FOLDR_EVAL7(__VA_ARGS__))
#define FOLDR_EVAL(...) FOLDR_EVAL8(__VA_ARGS__)
// FOLDR(f, a, b, c)             -> f(a, f(b, c))
// FOLDR(F, (a,b), (c,d), (e,f)) -> F((a,b), F((c,d), (e,f)))
// FOLDR(AND, 1, 1, 1, 1)        -> 1
// FOLDR(AND, 1, 1, 0, 1)        -> 0
// #define add(x,y) (x + y)
// FOLDR(add, 1, 2, 3)           -> (1 + (2 + 3))
// FOLDR(add, 1, 2, 3, 4)        -> (1 + (2 + (3 + 4)))
// FOLDR(add, 1, 2, 3, 4, 5)     -> (1 + (2 + (3 + (4 + 5))))

#define FOLDL(f, ...) FOLDL_EVAL(FOLDL_I(f, __VA_ARGS__, ))
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

#define APPLY(f, ...) APPLY_EVAL(APPLY_I(f, __VA_ARGS__, ))
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

#define SEP_BY(sep, f, ...) SEP_BY_EVAL(SEP_BY_I(sep, f, __VA_ARGS__, ))
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

#define FOR_EACH(f, ...) FOR_EACH_EVAL(FOR_EACH_I(f, __VA_ARGS__, ))
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
  GENERIC_SELECTORS_EVAL(GENERIC_SELECTORS_I(f, g, __VA_ARGS__, ))
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

#define SQUASH(...) TAIL(SQUASH_EVAL(SQUASH1(__VA_ARGS__, )))
#define SQUASH_INDIRECT() SQUASH1
#define SQUASH1(x, ...)                                                  \
  SQUASH0(x)                                                             \
  IF(IS_NIL(__VA_ARGS__))(, DEFER2(SQUASH_INDIRECT)()(__VA_ARGS__))
#define SQUASH0(x) IF(IS_NULL(x))(, DEFER2(COMMA)() x)
#define SQUASH_EVAL1(...) __VA_ARGS__
#define SQUASH_EVAL2(...) SQUASH_EVAL1(SQUASH_EVAL1(__VA_ARGS__))
#define SQUASH_EVAL3(...) SQUASH_EVAL2(SQUASH_EVAL2(__VA_ARGS__))
#define SQUASH_EVAL4(...) SQUASH_EVAL3(SQUASH_EVAL3(__VA_ARGS__))
#define SQUASH_EVAL5(...) SQUASH_EVAL4(SQUASH_EVAL4(__VA_ARGS__))
#define SQUASH_EVAL6(...) SQUASH_EVAL5(SQUASH_EVAL5(__VA_ARGS__))
#define SQUASH_EVAL7(...) SQUASH_EVAL6(SQUASH_EVAL6(__VA_ARGS__))
#define SQUASH_EVAL8(...) SQUASH_EVAL7(SQUASH_EVAL7(__VA_ARGS__))
#define SQUASH_EVAL(...) SQUASH_EVAL8(__VA_ARGS__)
// SQUASH(,,1,,2,3,,4,,,) -> 1, 2, 3, 4

#define PAIR(...) SQUASH(PAIR_EVAL(PAIR1(__VA_ARGS__, )))
#define PAIR_INDIRECT() PAIR1
#define PAIR1(x, ...)                                                    \
  COMMA()                                                                \
  PAIR0(x, __VA_ARGS__, )                                                \
  IF(IS_NIL(__VA_ARGS__))                                                \
  (, DEFER2(PAIR_INDIRECT)()(TAIL(__VA_ARGS__), ))
#define PAIR0(x, y, ...) IF(IS_NULL(y))(x, (x, y))
#define PAIR_EVAL1(...) __VA_ARGS__
#define PAIR_EVAL2(...) PAIR_EVAL1(PAIR_EVAL1(__VA_ARGS__))
#define PAIR_EVAL3(...) PAIR_EVAL2(PAIR_EVAL2(__VA_ARGS__))
#define PAIR_EVAL4(...) PAIR_EVAL3(PAIR_EVAL3(__VA_ARGS__))
#define PAIR_EVAL5(...) PAIR_EVAL4(PAIR_EVAL4(__VA_ARGS__))
#define PAIR_EVAL6(...) PAIR_EVAL5(PAIR_EVAL5(__VA_ARGS__))
#define PAIR_EVAL7(...) PAIR_EVAL6(PAIR_EVAL6(__VA_ARGS__))
#define PAIR_EVAL8(...) PAIR_EVAL7(PAIR_EVAL7(__VA_ARGS__))
#define PAIR_EVAL(...) PAIR_EVAL8(__VA_ARGS__)
// PAIR(1,2,3,4,5,6,7,8)   -> (1,2) , (3,4) , (5,6) , (7,8)
// PAIR(1,2,3,4,5,6,7,8,9) -> (1,2) , (3,4) , (5,6) , (7,8) , 9

#define TREE(...) TREE_EVAL(TREE1(__VA_ARGS__, ))
#define TREE_INDIRECT() TREE1
#define TREE1(x, ...)                                                    \
  IF(IS_NIL(__VA_ARGS__))                                                \
  (x, DEFER2(TREE_INDIRECT)()(PAIR(SQUASH(x, __VA_ARGS__)), ))
#define TREE_EVAL1(...) __VA_ARGS__
#define TREE_EVAL2(...) TREE_EVAL1(TREE_EVAL1(__VA_ARGS__))
#define TREE_EVAL3(...) TREE_EVAL2(TREE_EVAL2(__VA_ARGS__))
#define TREE_EVAL4(...) TREE_EVAL3(TREE_EVAL3(__VA_ARGS__))
#define TREE_EVAL5(...) TREE_EVAL4(TREE_EVAL4(__VA_ARGS__))
#define TREE_EVAL6(...) TREE_EVAL5(TREE_EVAL5(__VA_ARGS__))
#define TREE_EVAL7(...) TREE_EVAL6(TREE_EVAL6(__VA_ARGS__))
#define TREE_EVAL8(...) TREE_EVAL7(TREE_EVAL7(__VA_ARGS__))
#define TREE_EVAL(...) TREE_EVAL8(__VA_ARGS__)
// TREE(1) -> 1
// TREE(1,2) -> (1,2)
// TREE(1,2,3) -> ((1,2), 3)
// TREE(1,2,3,4) -> ((1,2), (3,4))
// TREE(1,2,3,4,5) -> (((1, 2), (3, 4)), 5)
// TREE(1,2,3,4,5,6,7,8,9) -> ((((1, 2), (3, 4)), ((5, 6), (7, 8))), 9)

#define TREE_COMPILE(f, tree) TREE_COMPILE_EVAL(TREE_COMPILE1(f, tree))
#define TREE_COMPILE_INDIRECT() TREE_COMPILE1
#define TREE_COMPILE1(f, tree)                                           \
  IF_ENCLOSED(tree)                                                      \
  (DEFER2(TREE_COMPILE_INDIRECT)()(f, FST tree)DEFER2(COMMA)() DEFER2(   \
       TREE_COMPILE_INDIRECT)()(f, SND tree)DEFER2(COMMA)()(NODE, f),    \
   (LEAF, tree))
#define TREE_COMPILE_EVAL1(...) __VA_ARGS__
#define TREE_COMPILE_EVAL2(...)                                          \
  TREE_COMPILE_EVAL1(TREE_COMPILE_EVAL1(__VA_ARGS__))
#define TREE_COMPILE_EVAL3(...)                                          \
  TREE_COMPILE_EVAL2(TREE_COMPILE_EVAL2(__VA_ARGS__))
#define TREE_COMPILE_EVAL4(...)                                          \
  TREE_COMPILE_EVAL3(TREE_COMPILE_EVAL3(__VA_ARGS__))
#define TREE_COMPILE_EVAL5(...)                                          \
  TREE_COMPILE_EVAL4(TREE_COMPILE_EVAL4(__VA_ARGS__))
#define TREE_COMPILE_EVAL6(...)                                          \
  TREE_COMPILE_EVAL5(TREE_COMPILE_EVAL5(__VA_ARGS__))
#define TREE_COMPILE_EVAL7(...)                                          \
  TREE_COMPILE_EVAL6(TREE_COMPILE_EVAL6(__VA_ARGS__))
#define TREE_COMPILE_EVAL8(...)                                          \
  TREE_COMPILE_EVAL7(TREE_COMPILE_EVAL7(__VA_ARGS__))
#define TREE_COMPILE_EVAL(...) TREE_COMPILE_EVAL8(__VA_ARGS__)
// TREE_COMPILE(a, TREE(1,2,3,4,5,6,7,8,9))
// -> (LEAF, 1), (LEAF, 2), (NODE, a),
//    (LEAF, 3), (LEAF, 4), (NODE, a),
//    (NODE, a),
//    (LEAF, 5), (LEAF, 6), (NODE, a),
//    (LEAF, 7), (LEAF, 8), (NODE, a),
//    (NODE, a),
//    (NODE, a),
//    (LEAF, 9),(NODE, a)

#define STACK(stack, x) STACK1(CAT(STACK_, FST x)(stack, SND x))
#define STACK1(...) __VA_ARGS__
#define STACK_LEAF STACK_PUSH
#define STACK_NODE STACK_OP2
#define STACK_PUSH(stack, x) (SQUASH(x, DISCLOSE(stack)))
#define STACK_OP2(stack, f) (f STACK_POP2 stack)
#define STACK_POP2(y, ...) STACK_POP2_(y, __VA_ARGS__, )
#define STACK_POP2_(y, x, ...) SQUASH((x, y), __VA_ARGS__)
// #define add(x, y) (x + y)
// STACK(, (PUSH, 1))                 -> (1)
// STACK((), (PUSH, 1))               -> (1)
// STACK((1), (PUSH, 2))              -> (2,1)
// STACK((2,1), (OP2, add))           -> (add (1,2)) -> ((1 + 2))
// STACK(((1 + 2)), (PUSH, 3))        -> (3, (1 + 2))
// STACK((3, (1 + 2)), (PUSH, 4))     -> (4, 3, (1 + 2))
// STACK((4, 3, (1 + 2)), (OP2, add)) -> ((3 + 4) , (1 + 2))

#define TREE_FOLDL(f, ...) TREE_FOLDL_PRIMITIVE(f, TREE(__VA_ARGS__))
#define TREE_FOLDL_PRIMITIVE(f, tree)                                    \
  DISCLOSE(TREE_FOLDL_EVAL(DEFER(TREE_FOLDL1)(, TREE_COMPILE(f, tree), )))
#define TREE_FOLDL_INDIRECT() TREE_FOLDL1
#define TREE_FOLDL1(stack, x, ...)                                       \
  IF(IS_NIL(__VA_ARGS__))                                                \
  (STACK(stack, x),                                                      \
   DEFER2(TREE_FOLDL_INDIRECT)()(STACK(stack, x), __VA_ARGS__))
#define TREE_FOLDL_EVAL1(...) __VA_ARGS__
#define TREE_FOLDL_EVAL2(...)                                            \
  TREE_FOLDL_EVAL1(TREE_FOLDL_EVAL1(__VA_ARGS__))
#define TREE_FOLDL_EVAL3(...)                                            \
  TREE_FOLDL_EVAL2(TREE_FOLDL_EVAL2(__VA_ARGS__))
#define TREE_FOLDL_EVAL4(...)                                            \
  TREE_FOLDL_EVAL3(TREE_FOLDL_EVAL3(__VA_ARGS__))
#define TREE_FOLDL_EVAL5(...)                                            \
  TREE_FOLDL_EVAL4(TREE_FOLDL_EVAL4(__VA_ARGS__))
#define TREE_FOLDL_EVAL6(...)                                            \
  TREE_FOLDL_EVAL5(TREE_FOLDL_EVAL5(__VA_ARGS__))
#define TREE_FOLDL_EVAL7(...)                                            \
  TREE_FOLDL_EVAL6(TREE_FOLDL_EVAL6(__VA_ARGS__))
#define TREE_FOLDL_EVAL8(...)                                            \
  TREE_FOLDL_EVAL7(TREE_FOLDL_EVAL7(__VA_ARGS__))
#define TREE_FOLDL_EVAL(...) TREE_FOLDL_EVAL8(__VA_ARGS__)
// #define add(x, y) (x + y)
// TREE_FOLDL(add, 1,2,3,4,5,6,7,8,9)
// -> ((((1 + 2) + (3 + 4)) + ((5 + 6) + (7 + 8))) + 9)
