/* -*- coding:utf-8-unix -*- */
#pragma once

#include "../alloc.h"
#include "../codegen.h"
#include "../macros.h"
#include "../none.h"
#include "../typeset.h"

// ---- List ----
// Name of List(T)
#define List(T) CAT(T, List)

// Name of functions for List(T)
#define LIST_BEGIN(T) CAT(List(T), _begin)
#define LIST_END(T) CAT(List(T), _end)
#define LIST_LENGTH(T) CAT(List(T), _length)

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

// -------------------------------------------------------------
// Concrete List(T) types

CPARSEC2_C_API_BEGIN

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

#ifdef CPARSEC2_USER_TYPESET
#define F(T) TYPEDEF_LIST(T, T)
FOREACH(F, SQUASH(CPARSEC2_USER_TYPESET));
#undef F
#endif

FOREACH(DECLARE_LIST, ELEMENT_TYPESET);

CPARSEC2_C_API_END

// -------------------------------------------------------------
// Generic functions for List(T)

#ifdef __cplusplus

#define DEFINE_CXX_LIST(T)                                               \
  inline auto list_begin(List(T) xs) {                                   \
    return LIST_BEGIN(T)(xs);                                            \
  }                                                                      \
  inline auto list_end(List(T) xs) {                                     \
    return LIST_END(T)(xs);                                              \
  }                                                                      \
  inline auto list_length(List(T) xs) {                                  \
    return LIST_LENGTH(T)(xs);                                           \
  }                                                                      \
  END_OF_STATEMENTS

FOREACH(DEFINE_CXX_LIST, ELEMENT_TYPESET);

#undef DEFINE_CXX_LIST

#else

#define list_begin(xs)                                                   \
  GENERIC_METHOD((xs), List, LIST_BEGIN, ELEMENT_TYPESET)(xs)
#define list_end(xs)                                                     \
  GENERIC_METHOD((xs), List, LIST_END, ELEMENT_TYPESET)(xs)
#define list_length(xs)                                                  \
  GENERIC_METHOD((xs), List, LIST_LENGTH, ELEMENT_TYPESET)(xs)

#endif
