/* -*- coding: utf-8-unix -*- */

#include "LList.h"

TYPEDEF_LList(String, const char*);
DECLARE_LList(String);
DEFINE_LList(String);

#define llist_head(xs) LList_HEAD(String)(xs)
#define llist_tail(xs) LList_TAIL(String)(xs)
#define llist_cons(x, xs) LList_CONS(String)(x, xs)

static LList(String) keywords = NULL;

bool is_keyword(const char* s) {
  for (LList(String) xs = keywords; xs; xs = llist_tail(xs)) {
    if (!strcmp(llist_head(xs), s)) {
      return true;
    }
  }
  return false;
}

void add_keyword(const char* s) {
  if (!is_keyword(s)) {
    keywords = llist_cons(s, keywords);
  }
}
