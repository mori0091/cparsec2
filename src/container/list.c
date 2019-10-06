/* -*- coding: utf-8-unix -*- */

#include <assert.h>
#include <string.h>

#include <cparsec2/container/buff.h>
#include <cparsec2/container/list.h>

// ---- List ----

// List(None)
DEFINE_LIST(None);

// List(String)
DEFINE_LIST(String);

// List(Int)
DEFINE_LIST(Int);

// List(Ptr)
DEFINE_LIST(Ptr);

// List(Node)
DEFINE_LIST(Node);

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
