/* -*- coding:utf-8-unix -*- */
#pragma once

// ---- Buffer (List builder) ----
// Name of Buff(T)
#define Buff(T) CAT(T, Buff)

// Name of functions for Buff(T)
#define BUFF_ENSURE_BY(T) CAT(buff_ensure_by_, T)
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
  void BUFF_ENSURE_BY(T)(Buff(T) * b, size_t sz);                        \
  void BUFF_ENSURE(T)(Buff(T) * b);                                      \
  void BUFF_PUSH(T)(Buff(T) * b, ELEMENT_TYPE(Buff(T)) x);               \
  void BUFF_APPEND(T)(Buff(T) * b, List(T) xs);                          \
  List(T) BUFF_FINISH(T)(Buff(T) * b)

#define DEFINE_BUFF(T)                                                   \
  DEFINE_BUFF_COMMON(T);                                                 \
  DEFINE_BUFF_FINISH(T)

#define DEFINE_BUFF_COMMON(T)                                            \
  void BUFF_ENSURE_BY(T)(Buff(T) * b, size_t sz) {                       \
    const size_t elem_size = sizeof(ELEMENT_TYPE(Buff(T)));              \
    if (!b->data) {                                                      \
      int cap = (sz + 7) / 8 * 8;                                        \
      if (0 < cap) {                                                     \
        b->len = 0;                                                      \
        b->capacity = cap;                                               \
        b->data = mem_malloc(elem_size * cap);                           \
      }                                                                  \
    } else {                                                             \
      int cap = b->len + sz;                                             \
      if (b->capacity < cap) {                                           \
        b->capacity = cap;                                               \
        b->data = mem_realloc(b->data, elem_size * cap);                 \
      }                                                                  \
    }                                                                    \
  }                                                                      \
  void BUFF_ENSURE(T)(Buff(T) * b) {                                     \
    BUFF_ENSURE_BY(T)(b, 1);                                             \
  }                                                                      \
  void BUFF_PUSH(T)(Buff(T) * b, ELEMENT_TYPE(Buff(T)) x) {              \
    BUFF_ENSURE(T)(b);                                                   \
    b->data[b->len++] = x;                                               \
  }                                                                      \
  void BUFF_APPEND(T)(Buff(T) * b, List(T) xs) {                         \
    ELEMENT_TYPE(List(T))* itr = LIST_BEGIN(T)(xs);                      \
    int n = LIST_LENGTH(T)(xs);                                          \
    BUFF_ENSURE_BY(T)(b, n);                                             \
    for (int i = 0; i < n; ++i) {                                        \
      b->data[b->len++] = *itr++;                                        \
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
