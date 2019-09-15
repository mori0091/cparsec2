/* -*- coding:utf-8-unix -*- */
#pragma once

typedef struct {
  char _;
} None;

static const None NONE = {0};

#ifdef __cplusplus
template <typename T>
inline auto isNONE(T) {
  return false;
}
template <>
inline auto isNONE(None) {
  return true;
}
#else
#define isNONE(x) _Generic((x), None : 1, default : 0)
#endif
