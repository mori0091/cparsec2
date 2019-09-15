/* -*- coding:utf-8-unix -*- */
#pragma once

#include "alloc.h"
#include "codegen.h"
#include "macros.h"
#include "none.h"

#ifdef __cplusplus
extern "C" {
#endif

#define ELEMENT_TYPE(T) CAT(T, _elem_type)

// ---- List ----
// Name of List(T)
#define List(T) CAT(T, List)

// Name of functions for List(T)
#define LIST_BEGIN(T) CAT(List(T), _begin)
#define LIST_END(T) CAT(List(T), _end)
#define LIST_LENGTH(T) CAT(List(T), _length)

// Generic functions for List(T)
#define list_begin(xs)                                                   \
  GENERIC_METHOD((xs), List, LIST_BEGIN, ELEMENT_TYPESET)(xs)
#define list_end(xs)                                                     \
  GENERIC_METHOD((xs), List, LIST_END, ELEMENT_TYPESET)(xs)
#define list_length(xs)                                                  \
  GENERIC_METHOD((xs), List, LIST_LENGTH, ELEMENT_TYPESET)(xs)

// Defines List(T) type whose element type is E, and
// defines type alias of E as ELEMENT_TYPE(List(T)) type.
#define TYPEDEF_LIST(T, E)                                               \
  typedef E ELEMENT_TYPE(List(T));                                       \
  typedef struct {                                                       \
    ELEMENT_TYPE(List(T)) * data;                                        \
    int len;                                                             \
  } List(T)

// Declares function prototypes for List(T)
#define DECLARE_LIST(T)                                                  \
  ELEMENT_TYPE(List(T)) * LIST_BEGIN(T)(List(T) xs);                     \
  ELEMENT_TYPE(List(T)) * LIST_END(T)(List(T) xs);                       \
  int LIST_LENGTH(T)(List(T) xs)

#define DEFINE_LIST(T)                                                   \
  ELEMENT_TYPE(List(T)) * LIST_BEGIN(T)(List(T) xs) {                    \
    return xs.data;                                                      \
  }                                                                      \
  ELEMENT_TYPE(List(T)) * LIST_END(T)(List(T) xs) {                      \
    return xs.data + xs.len;                                             \
  }                                                                      \
  int LIST_LENGTH(T)(List(T) xs) {                                       \
    return xs.len;                                                       \
  }                                                                      \
  END_OF_STATEMENTS

// List(Char)
#define CharList String
typedef const char* List(Char);
typedef const char ELEMENT_TYPE(List(Char));

// List(None)
TYPEDEF_LIST(None, None);

// List(String)
TYPEDEF_LIST(String, const char*);

// List(Int)
TYPEDEF_LIST(Int, int);

// List(Ptr)
TYPEDEF_LIST(Ptr, void*);

// List(Node)
TYPEDEF_LIST(Node, Node);

FOREACH(DECLARE_LIST, ELEMENT_TYPESET);

// ---- Buffer (List builder) ----
// Name of Buff(T)
#define Buff(T) CAT(T, Buff)

// Name of functions for Buff(T)
#define BUFF_ENSURE(T) CAT(buff_ensure_, T)
#define BUFF_PUSH(T) CAT(buff_push_, T)
#define BUFF_APPEND(T) CAT(buff_append_, T)
#define BUFF_FINISH(T) CAT(buff_finish_, T)

// Generic functions for Buff(T)
#define buff_ensure(b)                                                   \
  GENERIC_METHOD(*(b), Buff, BUFF_ENSURE, ELEMENT_TYPESET)(b)
#define buff_push(b, x)                                                  \
  GENERIC_METHOD(*(b), Buff, BUFF_PUSH, ELEMENT_TYPESET)(b, x)
#define buff_append(b, xs)                                               \
  GENERIC_METHOD(*(b), Buff, BUFF_APPEND, ELEMENT_TYPESET)(b, xs)
#define buff_finish(b)                                                   \
  GENERIC_METHOD(*(b), Buff, BUFF_FINISH, ELEMENT_TYPESET)(b)

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

#ifdef __cplusplus
}
#endif
