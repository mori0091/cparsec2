/* -*- coding:utf-8-unix -*- */

#include <string.h>

#include <cparsec2.h>

static char expects_fn(void* arg, Source src, Ctx* ex) {
  void** ps = (void**)arg;
  const char* desc = ps[0];
  CharParser p = ps[1];
  Ctx ctx;
  TRY(&ctx) {
    return parse(p, src, &ctx);
  }
  else {
    ErrMsg m = {Expect, desc};
    parseError(src, m);
    const char* bw = strstr(ctx.msg, "but was");
    if (bw) {
      cthrow(ex, error("expects %s %s", desc, bw));
    } else {
      cthrow(ex, ctx.msg);
    }
  }
}

CharParser expects(const char* desc, CharParser p) {
  void** ps = mem_malloc(sizeof(void*) * 2);
  ps[0] = (void*)desc;
  ps[1] = (void*)p;
  return PARSER_GEN(Char)(expects_fn, ps);
}
