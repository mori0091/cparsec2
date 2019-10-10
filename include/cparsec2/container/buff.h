/* -*- coding:utf-8-unix -*- */
#pragma once

#include "../alloc.h"
#include "../codegen.h"
#include "../macros.h"
#include "../none.h"
#include "../typeset.h"

#include "list.h"

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

// -------------------------------------------------------------
// Concrete Buff(T) types

CPARSEC2_C_API_BEGIN

// Buff(None)
TYPEDEF_BUFF(None, None);

// Buff(Char)
TYPEDEF_BUFF(Char, char);

// Buff(String)
TYPEDEF_BUFF(String, const char*);

// Buff(Int)
TYPEDEF_BUFF(Int, int);

// Buff(Ptr)
TYPEDEF_BUFF(Ptr, void*);

// Buff(Node)
TYPEDEF_BUFF(Node, Node);

FOREACH(DECLARE_BUFF, ELEMENT_TYPESET);

CPARSEC2_C_API_END

// -------------------------------------------------------------
// Generic functions for Buff(T)

#ifdef __cplusplus

#define DEFINE_CXX_BUFF(T)                                               \
  inline void buff_ensure_by(Buff(T) * b, size_t sz) {                   \
    BUFF_ENSURE_BY(T)(b, sz);                                            \
  }                                                                      \
  inline void buff_ensure(Buff(T) * b) {                                 \
    BUFF_ENSURE(T)(b);                                                   \
  }                                                                      \
  inline void buff_push(Buff(T) * b, ELEMENT_TYPE(Buff(T)) x) {          \
    BUFF_PUSH(T)(b, x);                                                  \
  }                                                                      \
  inline void buff_append(Buff(T) * b, List(T) xs) {                     \
    BUFF_APPEND(T)(b, xs);                                               \
  }                                                                      \
  inline auto buff_finish(Buff(T) * b) {                                 \
    return BUFF_FINISH(T)(b);                                            \
  }                                                                      \
  END_OF_STATEMENTS

FOREACH(DEFINE_CXX_BUFF, ELEMENT_TYPESET);

#undef DEFINE_CXX_BUFF

#else

#define buff_ensure_by(b, sz)                                            \
  GENERIC_METHOD(*(b), Buff, BUFF_ENSURE_BY, ELEMENT_TYPESET)(b, sz)
#define buff_ensure(b)                                                   \
  GENERIC_METHOD(*(b), Buff, BUFF_ENSURE, ELEMENT_TYPESET)(b)
#define buff_push(b, x)                                                  \
  GENERIC_METHOD(*(b), Buff, BUFF_PUSH, ELEMENT_TYPESET)(b, x)
#define buff_append(b, xs)                                               \
  GENERIC_METHOD(*(b), Buff, BUFF_APPEND, ELEMENT_TYPESET)(b, xs)
#define buff_finish(b)                                                   \
  GENERIC_METHOD(*(b), Buff, BUFF_FINISH, ELEMENT_TYPESET)(b)

#endif

// -------------------------------------------------------------
#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Appends a printf-like formatted string.
 *
 * \param b       pointer to a char buffer
 * \param format  format string
 * \param ...     arguments for formatting
 *
 * \see printf(3)
 */
void buff_printf(Buff(Char) * b, const char* format, ...);

void buff_vprintf(Buff(Char) * b, const char* format, va_list ap);

#ifdef __cplusplus
}
#endif
