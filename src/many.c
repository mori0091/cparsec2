/* -*- coding:utf-8-unix -*- */

#include "cparsec2.h"

static const char* run_many(void* arg, Source src, Ctx* ex) {
  UNUSED(ex);
  CharParser parser = (CharParser)arg;
  Buffer str = buf_new();
  Ctx ctx;
  TRY(&ctx) {
    for (;;) {
      buf_push(&str, parse(parser, src, &ctx));
    }
  }
  else {
    /* catch and discard exception */
    mem_free((void*)ctx.msg);
  }
  return buf_finish(&str);
}

StringParser many(CharParser p) { return genParser(run_many, p); }
