/* -*- coding:utf-8-unix -*- */
#pragma once

#include <cparsec2/list.h>

// Generic functions for List(T)

#ifdef list_begin
#undef list_begin
#endif

#ifdef list_end
#undef list_end
#endif

#ifdef list_length
#undef list_length
#endif

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

// Generic functions for Buff(T)

#ifdef buff_ensure
#undef buff_ensure
#endif

#ifdef buff_push
#undef buff_push
#endif

#ifdef buff_append
#undef buff_append
#endif

#ifdef buff_finish
#undef buff_finish
#endif

#define DEFINE_CXX_BUFF(T)                                               \
  inline void buff_ensure(Buff(T) * b) {                                 \
    BUFF_ENSURE(T)(b);                                                   \
  }                                                                      \
  template <typename E>                                                  \
  inline void buff_push(Buff(T) * b, E x) {                              \
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
