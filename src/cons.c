/* -*- coding:utf-8-unix -*- */

#include "cparsec2.h"

struct cons_arg {
  PARSER(Char) head;
  PARSER(String) tail;
};

static const char* run_cons_char(void* arg, Source src, Ctx* ex) {
  struct cons_arg* self = (struct cons_arg*)arg;
  Buff(Char) str = {0};
  Ctx ctx;
  TRY(&ctx) {
    buff_push(&str, parse(self->head, src, &ctx));
    buff_append(&str, parse(self->tail, src, &ctx));
    return buff_finish(&str);
  }
  else {
    mem_free(str.data);
    cthrow(ex, ctx.msg);
  }
}

PARSER(String) cons_char(PARSER(Char) p, PARSER(String) ps) {
  struct cons_arg* arg = mem_malloc(sizeof(struct cons_arg));
  arg->head = p;
  arg->tail = ps;
  return PARSER_GEN(String)(run_cons_char, arg);
}
