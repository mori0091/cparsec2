/* -*- coding:utf-8-unix -*- */

#include <cparsec2.h>

struct satisfy_arg {
  Predicate pred;
};

static char run_satisfy(void* arg, Source src, Ctx* ex) {
  struct satisfy_arg* self = (struct satisfy_arg*)arg;
  Ctx ctx;
  TRY(&ctx) {
    char c = peek(src, &ctx);
    if (self->pred(c)) {
      consume(src);
      return c;
    }
    ErrMsg m = {SysUnexpect, mem_printf("'%c'", c)};
    parseError(src, m);
    cthrow(&ctx, error("not satisfy"));
  }
  else {
    cthrow(ex, ctx.msg);
  }
}

PARSER(Char) satisfy(Predicate pred) {
  struct satisfy_arg* arg = mem_malloc(sizeof(struct satisfy_arg));
  arg->pred = pred;
  return PARSER_GEN(Char)(run_satisfy, arg);
}
