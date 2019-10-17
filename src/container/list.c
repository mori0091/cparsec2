/* -*- coding: utf-8-unix -*- */

#include <cparsec2.h>

#include <assert.h>
#include <string.h>

// ---- List ----

// List(Char) ; a special case of List(T)
ELEMENT_TYPE(List(Char)) * LIST_BEGIN(Char)(List(Char) xs) {
  assert(xs);
  return xs;
}
ELEMENT_TYPE(List(Char)) * LIST_END(Char)(List(Char) xs) {
  assert(xs);
  return xs + LIST_LENGTH(Char)(xs);
}
int LIST_LENGTH(Char)(List(Char) xs) {
  assert(xs);
  size_t n = strnlen(xs, SIZE_MAX);
  assert(n < SIZE_MAX);
  return n;
}
