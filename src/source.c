/* -*- coding:utf-8-unix -*- */

#include <cparsec2/source.h>

#include <cparsec2/alloc.h>
#include <cparsec2/stream.h>

// ---- source of input character sequence ----

struct stSource {
  Stream s;
};

Source newStringSource(const char* text) {
  Source src = mem_malloc(sizeof(struct stSource));
  src->s = Stream_new(text);
  return src;
}

Source newFileSource(FILE* fp) {
  Source src = mem_malloc(sizeof(struct stSource));
  src->s = Stream_new(fp);
  return src;
}

char peek(Source src, Ctx* ctx) {
  char c;
  off_t pos = Stream_getpos(src->s, ctx);
  if (!Stream_read((void*)&c, 1, src->s, ctx)) {
    cthrow(ctx, mem_printf("too short"));
  }
  Stream_setpos(pos, src->s, ctx);
  return c;
}

void consume(Source src) {
  Ctx ctx;
  TRY(&ctx) {
    char c;
    if (!Stream_read((void*)&c, 1, src->s, &ctx)) {
      cthrow(&ctx, mem_printf("too short"));
    }
  }
  else {
    fprintf(stderr, "%s\n", ctx.msg);
    exit(1);
  }
}

SourcePos getSourcePos(Source src) {
  Ctx ctx;
  TRY(&ctx) {
    return (SourcePos){.offset = Stream_getpos(src->s, &ctx)};
  }
  else {
    fprintf(stderr, "%s\n", ctx.msg);
    exit(1);
  }
}

void setSourcePos(Source src, SourcePos pos) {
  Ctx ctx;
  TRY(&ctx) {
    Stream_setpos(pos.offset, src->s, &ctx);
  }
  else {
    fprintf(stderr, "%s\n", ctx.msg);
    exit(1);
  }
}
