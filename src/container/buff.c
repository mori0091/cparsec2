/* -*- coding: utf-8-unix -*- */

#include <assert.h>
#include <string.h>

#include <cparsec2/container/buff.h>
#include <cparsec2/container/list.h>

// ---- Buffer (List builder) ----

// Buff(Ptr)
DEFINE_BUFF(Ptr);

// Buff(Char) ; a special case of Buff(T)
DEFINE_BUFF_COMMON(Char);
List(Char) BUFF_FINISH(Char)(Buff(Char) * b) {
  BUFF_PUSH(Char)(b, '\0');
  List(Char) xs = b->data;
  *b = (Buff(Char)){0};
  return xs;
}

// Define Buff(T) for each remaining T in ELEMENT_TYPESET.
FOREACH(DEFINE_BUFF, TYPESET_0());
