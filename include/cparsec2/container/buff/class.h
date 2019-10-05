/* -*- coding:utf-8-unix -*- */
#pragma once

// ---- Buffer (List builder) ----
// Name of Buff(T)
#define Buff(T) CAT(T, Buff)

// Name of functions for Buff(T)
#define BUFF_ENSURE(T) CAT(buff_ensure_, T)
#define BUFF_PUSH(T) CAT(buff_push_, T)
#define BUFF_APPEND(T) CAT(buff_append_, T)
#define BUFF_FINISH(T) CAT(buff_finish_, T)

// Defines Buff(T) type whose element type is E, and
// defines type alias of E as ELEMENT_TYPE(Buff(T)) type.
#define TYPEDEF_BUFF(T, E)                                               \
  typedef E ELEMENT_TYPE(Buff(T));                                       \
  typedef struct {                                                       \
    ELEMENT_TYPE(Buff(T)) * data;                                        \
    int capacity;                                                        \
    int len;                                                             \
  } Buff(T)

// Declares function prototypes for Buff(T)
#define DECLARE_BUFF(T)                                                  \
  void BUFF_ENSURE(T)(Buff(T) * b);                                      \
  void BUFF_PUSH(T)(Buff(T) * b, ELEMENT_TYPE(Buff(T)) x);               \
  void BUFF_APPEND(T)(Buff(T) * b, List(T) xs);                          \
  List(T) BUFF_FINISH(T)(Buff(T) * b)

#define DEFINE_BUFF(T)                                                   \
  DEFINE_BUFF_COMMON(T);                                                 \
  DEFINE_BUFF_FINISH(T)

#define DEFINE_BUFF_COMMON(T)                                            \
  void BUFF_ENSURE(T)(Buff(T) * b) {                                     \
    if (!b->data) {                                                      \
      b->data = mem_malloc(sizeof(ELEMENT_TYPE(Buff(T))) * 8);           \
      b->capacity = 8;                                                   \
      b->len = 0;                                                        \
    }                                                                    \
    if (b->capacity == b->len) {                                         \
      b->capacity *= 2;                                                  \
      b->data = mem_realloc(b->data, sizeof(ELEMENT_TYPE(Buff(T))) *     \
                                         b->capacity);                   \
    }                                                                    \
  }                                                                      \
  void BUFF_PUSH(T)(Buff(T) * b, ELEMENT_TYPE(Buff(T)) x) {              \
    BUFF_ENSURE(T)(b);                                                   \
    b->data[b->len++] = x;                                               \
  }                                                                      \
  void BUFF_APPEND(T)(Buff(T) * b, List(T) xs) {                         \
    ELEMENT_TYPE(List(T))* itr = LIST_BEGIN(T)(xs);                      \
    ELEMENT_TYPE(List(T))* end = LIST_END(T)(xs);                        \
    for (; itr != end; itr++) {                                          \
      BUFF_PUSH(T)(b, *itr);                                             \
    }                                                                    \
  }                                                                      \
  END_OF_STATEMENTS

#define DEFINE_BUFF_FINISH(T)                                            \
  List(T) BUFF_FINISH(T)(Buff(T) * b) {                                  \
    List(T) xs = (List(T)){.data = b->data, .len = b->len};              \
    *b = (Buff(T)){0};                                                   \
    return xs;                                                           \
  }                                                                      \
  END_OF_STATEMENTS
