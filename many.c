/* -*- coding:utf-8-unix -*- */

#include "cparsec2.h"

static StringResult run_many(void *arg, Source src) {
  CharParser parser = (CharParser)arg;
  Buffer str = buf_new();
  Ctx ctx;
  TRY(&ctx) {
    for (;;) {
      buf_push(&str, parseEx(parser, src, &ctx));
    }
  } else {
    /* catch and discard exception */
    free((void *)ctx.msg);
  }
  return (StringResult){.result = buf_finish(&str)};
}

StringParser many(CharParser p) {
  return genParser(run_many, p);
}
