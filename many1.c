/* -*- coding:utf-8-unix -*- */

#include "cparsec2.h"

static StringResult run_many1(void *arg, Source src) {
  CharParser parser = (CharParser)arg;
  Ctx ctx;
  Buffer str = buf_new();
  TRY(&ctx) {
    buf_push(&str, parseEx(parser, src, &ctx));
    buf_append(&str, parseEx(many(parser), src, &ctx));
    return (StringResult){.result = buf_finish(&str)};
  } else {
    free((void *)str.data);
    /* catch and re-throw exception */
    return (StringResult){.error = ctx.msg};
  }
}

StringParser many1(CharParser p) {
  return genParser(run_many1, p);
}
