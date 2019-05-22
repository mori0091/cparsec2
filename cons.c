/* -*- coding:utf-8-unix -*- */

#include "cparsec2.h"

struct cons_arg {
  CharParser head;
  StringParser tail;
};

static StringResult run_cons_char(void *arg, Source src) {
  struct cons_arg *self = (struct cons_arg *)arg;
  Buffer str = buf_new();
  Ctx ctx;
  TRY(&ctx) {
    buf_push(&str, parseEx(self->head, src, &ctx));
    buf_append(&str, parseEx(self->tail, src, &ctx));
    return (StringResult){.result = buf_finish(&str)};
  } else {
    mem_free(str.data);
    return (StringResult){.error = ctx.msg};
  }
}

StringParser cons_char(CharParser p, StringParser ps) {
  struct cons_arg *arg = mem_malloc(sizeof(struct cons_arg));
  arg->head = p;
  arg->tail = ps;
  return genParser(run_cons_char, arg);
}
