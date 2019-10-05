/* -*- coding:utf-8-unix -*- */
#pragma once

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
