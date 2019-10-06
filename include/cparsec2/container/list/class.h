/* -*- coding:utf-8-unix -*- */
#pragma once

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
