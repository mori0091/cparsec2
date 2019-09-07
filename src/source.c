/* -*- coding:utf-8-unix -*- */

#include <cparsec2/source.h>

#include <cparsec2/alloc.h>
#include <cparsec2/stream.h>

// ---- source of input character sequence ----

struct stSource {
  Stream s;
  ParseError e;
};

const char* getParseErrorAsString(Source src) {
  return ParseError_toString(getParseError(src));
}

ParseError getParseError(Source src) {
  return src->e;
}
void setParseError(Source src, ParseError err) {
  src->e = err;
}

void parseError(Source src, ErrMsg msg) {
  SourcePos pos = getSourcePos(src);
  ParseError err = getParseError(src);
  err = ParseError_setMessage(msg, err);
  err = ParseError_setPos(pos, err);
  setParseError(src, err);
}

Source newStringSource(const char* text) {
  Source src = mem_malloc(sizeof(struct stSource));
  src->s = Stream_new(text);
  src->e = ParseError_new(getSourcePos(src));
  return src;
}

Source newFileSource(FILE* fp) {
  Source src = mem_malloc(sizeof(struct stSource));
  src->s = Stream_new(fp);
  src->e = ParseError_new(getSourcePos(src));
  return src;
}

static char readChar(Source src, Ctx* ctx) {
  char c;
  if (!Stream_read((void*)&c, 1, src->s, ctx)) {
    parseError(src, (ErrMsg){Unexpect, "end of input"});
    cthrow(ctx, mem_printf("too short"));
  }
  return c;
}

char peek(Source src, Ctx* ctx) {
  off_t pos = Stream_getpos(src->s, ctx);
  char c = readChar(src, ctx);
  Stream_setpos(pos, src->s, ctx);
  return c;
}

void consume(Source src) {
  Ctx ctx;
  TRY(&ctx) {
    readChar(src, &ctx);
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
