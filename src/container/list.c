/* -*- coding: utf-8-unix -*- */

#include <assert.h>
#include <string.h>

#include <cparsec2/container/buff.h>
#include <cparsec2/container/list.h>

// ---- List ----

// List(Ptr)
DEFINE_LIST(Ptr);

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

// Define List(T) for each remaining T in ELEMENT_TYPESET.
FOREACH(DEFINE_LIST, TYPESET_0());
