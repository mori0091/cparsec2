/* -*- coding:utf-8-unix -*- */
#pragma once

#include <cparsec2.h>

#ifdef __cplusplus
extern "C" {
#endif

#define LList(T) CAT(T, LinkedList)
#define LListSt(T) CAT(LList(T), St)

#define LList_CONS(T) CAT(LList(T), _cons)
#define LList_HEAD(T) CAT(LList(T), _head)
#define LList_TAIL(T) CAT(LList(T), _tail)

#define TYPEDEF_LList(T, E)                                              \
  typedef E ELEMENT_TYPE(LList(T));                                      \
  typedef struct LListSt(T) * LList(T);                                  \
  struct LListSt(T) {                                                    \
    ELEMENT_TYPE(LList(T)) head;                                         \
    LList(T) tail;                                                       \
  }

#define DECLARE_LList(T)                                                 \
  LList(T) LList_CONS(T)(ELEMENT_TYPE(LList(T)) x, LList(T) xs);         \
  ELEMENT_TYPE(LList(T)) LList_HEAD(T)(LList(T) xs);                     \
  LList(T) LList_TAIL(T)(LList(T) xs)

#define DEFINE_LList(T)                                                  \
  LList(T) LList_CONS(T)(ELEMENT_TYPE(LList(T)) x, LList(T) xs) {        \
    LList(T) xxs = mem_malloc(sizeof(struct LListSt(T)));                \
    xxs->head = x;                                                       \
    xxs->tail = xs;                                                      \
    return xxs;                                                          \
  }                                                                      \
  ELEMENT_TYPE(LList(T)) LList_HEAD(T)(LList(T) xs) {                    \
    assert(xs);                                                          \
    return xs->head;                                                     \
  }                                                                      \
  LList(T) LList_TAIL(T)(LList(T) xs) {                                  \
    assert(xs);                                                          \
    return xs->tail;                                                     \
  }                                                                      \
  END_OF_STATEMENTS

#ifdef __cplusplus
}
#endif
