/* -*- coding:utf-8-unix -*- */

#include "cparsec2.h"

struct satisfy_arg {
  Predicate pred;
};

static CharResult run_satisfy(void *arg, Source src) {
  struct satisfy_arg *self = (struct satisfy_arg *)arg;
  Ctx ctx;
  TRY(&ctx) {
    char c = peek(src, &ctx);
    if (self->pred(c)) {
      consume(src);
      return (CharResult){.result = c};
    }
    raise(&ctx, error("not satisfy"));
  } else {
    return (CharResult){.error = ctx.msg};
  }
}

CharParser satisfy(Predicate pred) {
  struct satisfy_arg *arg = malloc(sizeof(struct satisfy_arg));
  arg -> pred = pred;
  return genParser(run_satisfy, arg);
}
