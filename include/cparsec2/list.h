/* -*- coding:utf-8-unix -*- */
#pragma once

#include "macros.h"

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
#define list_begin(xs) GENERIC_METHOD(xs, List, LIST_BEGIN)(xs)
#define list_end(xs) GENERIC_METHOD(xs, List, LIST_END)(xs)
#define list_length(xs) GENERIC_METHOD(xs, List, LIST_LENGTH)(xs)

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

// List(Char)
#define CharList String
typedef const char* List(Char);
typedef const char ELEMENT_TYPE(List(Char));
DECLARE_LIST(Char);

// List(String)
TYPEDEF_LIST(String, const char*);
DECLARE_LIST(String);

// List(Int)
TYPEDEF_LIST(Int, int);
DECLARE_LIST(Int);

// List(Ptr)
TYPEDEF_LIST(Ptr, void*);
DECLARE_LIST(Ptr);

// ---- Buffer (List builder) ----
// Name of Buff(T)
#define Buff(T) CAT(T, Buff)

// Name of functions for Buff(T)
#define BUFF_ENSURE(T) CAT(buff_ensure_, T)
#define BUFF_PUSH(T) CAT(buff_push_, T)
#define BUFF_APPEND(T) CAT(buff_append_, T)
#define BUFF_FINISH(T) CAT(buff_finish_, T)

// Generic functions for Buff(T)
#define buff_ensure(b) GENERIC_METHOD(*(b), Buff, BUFF_ENSURE)(b)
#define buff_push(b, x) GENERIC_METHOD(*(b), Buff, BUFF_PUSH)(b, x)
#define buff_append(b, xs) GENERIC_METHOD(*(b), Buff, BUFF_APPEND)(b, xs)
#define buff_finish(b) GENERIC_METHOD(*(b), Buff, BUFF_FINISH)(b)

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

// Buff(Char)
TYPEDEF_BUFF(Char, char);
DECLARE_BUFF(Char);

// Buff(String)
TYPEDEF_BUFF(String, const char*);
DECLARE_BUFF(String);

// Buff(Int)
TYPEDEF_BUFF(Int, int);
DECLARE_BUFF(Int);

// Buff(Ptr)
TYPEDEF_BUFF(Ptr, void*);
DECLARE_BUFF(Ptr);

#ifdef __cplusplus
}
#endif
