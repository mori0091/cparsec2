/* -*- coding:utf-8-unix -*- */
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

// ---- List ----
#define List(T) T##List
#define LIST_BEGIN(T) list_begin_##T
#define LIST_END(T) list_end_##T
#define LIST_LENGTH(T) list_length_##T

#define TYPEDEF_LIST(T, E)                                               \
  typedef struct {                                                       \
    E* data;                                                             \
    int len;                                                             \
  } List(T)

#define DECLARE_LIST(T, E)                                               \
  E* LIST_BEGIN(T)(List(T) xs);                                          \
  E* LIST_END(T)(List(T) xs);                                            \
  int LIST_LENGTH(T)(List(T) xs)

// List(Char)
#define CharList String
typedef const char* List(Char);
DECLARE_LIST(Char, const char);

// List(String)
TYPEDEF_LIST(String, const char*);
DECLARE_LIST(String, const char*);

// List(Int)
TYPEDEF_LIST(Int, int);
DECLARE_LIST(Int, int);

// List(Ptr)
TYPEDEF_LIST(Ptr, void*);
DECLARE_LIST(Ptr, void*);

// clang-format off
#define list_begin(xs)                          \
  _Generic((xs)                                 \
           , List(Char)   : LIST_BEGIN(Char)    \
           , List(String) : LIST_BEGIN(String)  \
           , List(Int)    : LIST_BEGIN(Int)     \
           , List(Ptr)    : LIST_BEGIN(Ptr)     \
           )(xs)
#define list_end(xs)                            \
  _Generic((xs)                                 \
           , List(Char)   : LIST_END(Char)      \
           , List(String) : LIST_END(String)    \
           , List(Int)    : LIST_END(Int)       \
           , List(Ptr)    : LIST_END(Ptr)       \
           )(xs)
#define list_length(xs)                         \
  _Generic((xs)                                 \
           , List(Char)   : LIST_LENGTH(Char)   \
           , List(String) : LIST_LENGTH(String) \
           , List(Int)    : LIST_LENGTH(Int)    \
           , List(Ptr)    : LIST_LENGTH(Ptr)    \
           )(xs)
// clang-format on

// ---- Buffer (List builder) ----
#define Buff(T) T##Buff
#define BUFF_ENSURE(T) buff_ensure_##T
#define BUFF_PUSH(T) buff_push_##T
#define BUFF_APPEND(T) buff_append_##T
#define BUFF_FINISH(T) buff_finish_##T

#define DECLARE_BUFF(T, E)                                               \
  typedef struct {                                                       \
    E* data;                                                             \
    int capacity;                                                        \
    int len;                                                             \
  } Buff(T);                                                             \
  void BUFF_ENSURE(T)(Buff(T) * b);                                      \
  void BUFF_PUSH(T)(Buff(T) * b, E x);                                   \
  void BUFF_APPEND(T)(Buff(T) * b, List(T) xs);                          \
  List(T) BUFF_FINISH(T)(Buff(T) * b)

DECLARE_BUFF(Char, char);
DECLARE_BUFF(String, const char*);
DECLARE_BUFF(Int, int);
DECLARE_BUFF(Ptr, void*);

// clang-format off
#define buff_ensure(b)                          \
  _Generic((*(b))                               \
           , Buff(Char)   : BUFF_ENSURE(Char)   \
           , Buff(String) : BUFF_ENSURE(String) \
           , Buff(Int)    : BUFF_ENSURE(Int)    \
           , Buff(Ptr)    : BUFF_ENSURE(Ptr)    \
           )(b)
#define buff_push(b, x)                         \
  _Generic((*(b))                               \
           , Buff(Char)   : BUFF_PUSH(Char)     \
           , Buff(String) : BUFF_PUSH(String)   \
           , Buff(Int)    : BUFF_PUSH(Int)      \
           , Buff(Ptr)    : BUFF_PUSH(Ptr)      \
           )(b, x)
#define buff_append(b, xs)                      \
  _Generic((*(b))                               \
           , Buff(Char)   : BUFF_APPEND(Char)   \
           , Buff(String) : BUFF_APPEND(String) \
           , Buff(Int)    : BUFF_APPEND(Int)    \
           , Buff(Ptr)    : BUFF_APPEND(Ptr)    \
           )(b, xs)
#define buff_finish(b)                          \
  _Generic((*(b))                               \
           , Buff(Char)   : BUFF_FINISH(Char)   \
           , Buff(String) : BUFF_FINISH(String) \
           , Buff(Int)    : BUFF_FINISH(Int)    \
           , Buff(Ptr)    : BUFF_FINISH(Ptr)    \
           )(b)
// clang-format on

#ifdef __cplusplus
}
#endif
