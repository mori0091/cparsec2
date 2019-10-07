/* -*- coding:utf-8-unix -*- */
#pragma once

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
