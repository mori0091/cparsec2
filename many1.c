/* -*- coding:utf-8-unix -*- */

#include "cparsec2.h"

static const char *run_many1(void *arg, Source src, Ctx *ex) {
  CharParser parser = (CharParser)arg;
  Ctx ctx;
  Buffer str = buf_new();
  TRY(&ctx) {
    buf_push(&str, parse(parser, src, &ctx));
    buf_append(&str, parse(many(parser), src, &ctx));
    return buf_finish(&str);
  } else {
    mem_free((void *)str.data);
    /* catch and re-throw exception */
    raise(ex, ctx.msg);
  }
}

StringParser many1(CharParser p) {
  return genParser(run_many1, p);
}
