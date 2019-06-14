/* -*- coding:utf-8-unix -*- */
#pragma once

#include <cparsec2/list.h>

#ifdef list_begin
#undef list_begin
inline auto list_begin(List(Char) xs) {
  return LIST_BEGIN(Char)(xs);
}
inline auto list_begin(List(String) xs) {
  return LIST_BEGIN(String)(xs);
}
inline auto list_begin(List(Int) xs) {
  return LIST_BEGIN(Int)(xs);
}
inline auto list_begin(List(Ptr) xs) {
  return LIST_BEGIN(Ptr)(xs);
}
#endif

#ifdef list_end
#undef list_end
inline auto list_end(List(Char) xs) {
  return LIST_END(Char)(xs);
}
inline auto list_end(List(String) xs) {
  return LIST_END(String)(xs);
}
inline auto list_end(List(Int) xs) {
  return LIST_END(Int)(xs);
}
inline auto list_end(List(Ptr) xs) {
  return LIST_END(Ptr)(xs);
}
#endif

#ifdef list_length
#undef list_length
inline auto list_length(List(Char) xs) {
  return LIST_LENGTH(Char)(xs);
}
inline auto list_length(List(String) xs) {
  return LIST_LENGTH(String)(xs);
}
inline auto list_length(List(Int) xs) {
  return LIST_LENGTH(Int)(xs);
}
inline auto list_length(List(Ptr) xs) {
  return LIST_LENGTH(Ptr)(xs);
}
#endif

#ifdef buff_ensure
#undef buff_ensure
inline void buff_ensure(Buff(Char)* b) {
  BUFF_ENSURE(Char)(b);
}
inline void buff_ensure(Buff(String)* b) {
  BUFF_ENSURE(String)(b);
}
inline void buff_ensure(Buff(Int)* b) {
  BUFF_ENSURE(Int)(b);
}
inline void buff_ensure(Buff(Ptr)* b) {
  BUFF_ENSURE(Ptr)(b);
}
#endif

#ifdef buff_push
#undef buff_push
inline void buff_push(Buff(Char)* b, char x) {
  BUFF_PUSH(Char)(b, x);
}
inline void buff_push(Buff(String)* b, const char* x) {
  BUFF_PUSH(String)(b, x);
}
inline void buff_push(Buff(Int)* b, int x) {
  BUFF_PUSH(Int)(b, x);
}
inline void buff_push(Buff(Ptr)* b, void* x) {
  BUFF_PUSH(Ptr)(b, x);
}
#endif

#ifdef buff_append
#undef buff_append
inline void buff_append(Buff(Char)* b, List(Char) xs) {
  BUFF_APPEND(Char)(b, xs);
}
inline void buff_append(Buff(String)* b, List(String) xs) {
  BUFF_APPEND(String)(b, xs);
}
inline void buff_append(Buff(Int)* b, List(Int) xs) {
  BUFF_APPEND(Int)(b, xs);
}
inline void buff_append(Buff(Ptr)* b, List(Ptr) xs) {
  BUFF_APPEND(Ptr)(b, xs);
}
#endif

#ifdef buff_finish
#undef buff_finish
inline auto buff_finish(Buff(Char)* b) {
  return BUFF_FINISH(Char)(b);
}
inline auto buff_finish(Buff(String)* b) {
  return BUFF_FINISH(String)(b);
}
inline auto buff_finish(Buff(Int)* b) {
  return BUFF_FINISH(Int)(b);
}
inline auto buff_finish(Buff(Ptr)* b) {
  return BUFF_FINISH(Ptr)(b);
}
#endif
