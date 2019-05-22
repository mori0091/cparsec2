/* -*- coding:utf-8-unix -*- */

#include "cparsec2.h"

struct cons_arg {
  CharParser head;
  StringParser tail;
};

static const char *run_cons_char(void *arg, Source src, Ctx *ex) {
  struct cons_arg *self = (struct cons_arg *)arg;
  Buffer str = buf_new();
  Ctx ctx;
  TRY(&ctx) {
    buf_push(&str, parse(self->head, src, &ctx));
    buf_append(&str, parse(self->tail, src, &ctx));
    return buf_finish(&str);
  } else {
    mem_free(str.data);
    raise(ex, ctx.msg);
  }
}

StringParser cons_char(CharParser p, StringParser ps) {
  struct cons_arg *arg = mem_malloc(sizeof(struct cons_arg));
  arg->head = p;
  arg->tail = ps;
  return genParser(run_cons_char, arg);
}
