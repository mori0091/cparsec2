/* -*- coding:utf-8-unix -*- */

#include "cparsec2.h"

static StringResult run_seq_char(void *arg, Source src) {
  CharParser *p = (CharParser *)arg;
  Buffer str = buf_new();
  Ctx ctx;
  TRY(&ctx) {
    while (*p) {
      buf_push(&str, parse(*p++, src, &ctx));
    }
    return (StringResult){.result = buf_finish(&str)};
  } else {
    mem_free((void *)str.data);
    /* catch and re-throw exception */
    return (StringResult){.error = ctx.msg};
  }
}

StringParser seq_char(CharParser ps[]) {
  PtrBuffer buf = ptrbuf_new();
  while (*ps) {
    ptrbuf_push(&buf, *ps++);
  }
  return genParser(run_seq_char, ptrbuf_finish(&buf));
}
