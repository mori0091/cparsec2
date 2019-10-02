/* -*- coding: utf-8-unix -*- */

#include <string.h>

#include "LList.h"
#include "LVar.h"

TYPEDEF_LList(LVar, LVar);
DECLARE_LList(LVar);
DEFINE_LList(LVar);

#define llist_head(xs) LList_HEAD(LVar)(xs)
#define llist_tail(xs) LList_TAIL(LVar)(xs)
#define llist_cons(x, xs) LList_CONS(LVar)(x, xs)

// linked-list of local variables
static LList(LVar) locals = NULL;

void clearLVars(void) {
  while (locals) {
    LList(LVar) xs = locals;
    locals = llist_tail(locals);
    mem_free(xs);
  }
}

int getLVarOffsetMax(void) {
  if (!locals) {
    return 0;
  } else {
    LVar x = llist_head(locals);
    return x.offset + x.size;
  }
}

LVar getLVar(const char* name, Ctx* ex) {
  int length = list_length(name);
  for (LList(LVar) xs = locals; xs; xs = llist_tail(xs)) {
    LVar x = llist_head(xs);
    if (length == x.length && strncmp(name, x.name, length) == 0) {
      return x;
    }
  }
  cthrow(ex, error("undefined local variable :%s", name));
}

LVar addLVar(const char* name) {
  LVar x = (LVar){.name = name,
                  .length = list_length(name),
                  .offset = getLVarOffsetMax(),
                  .size = 8};
  locals = llist_cons(x, locals);
  return x;
}

bool hasLVar(const char* name) {
  Ctx ctx;
  TRY(&ctx) {
    getLVar(name, &ctx);
    return true;
  }
  mem_free((void*)ctx.msg);
  return false;
}

// find local variable or register if not found
LVar findLVar(const char* name) {
  Ctx ctx;
  TRY(&ctx) {
    return getLVar(name, &ctx);
  }
  else {
    mem_free((void*)ctx.msg);
    return addLVar(name);
  }
}
