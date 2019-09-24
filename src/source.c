/* -*- coding:utf-8-unix -*- */

#include <cparsec2/source.h>

#include <cparsec2/alloc.h>
#include <cparsec2/stream.h>

// ---- source of input character sequence ----

struct stSource {
  Stream s;
  SourcePos pos;
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
  intmax_t offset = getSourceOffset(src);
  SourcePos pos = getSourcePos(src);
  ParseError err = getParseError(src);
  err = ParseError_setMessage(msg, err);
  err = ParseError_setOffset(offset, err);
  err = ParseError_setPos(pos, err);
  setParseError(src, err);
}

Source Source_identity(Source src) {
  return src;
}

Source newStringSource(const char* text) {
  Source src = mem_malloc(sizeof(struct stSource));
  src->s = Stream_new(text);
  src->pos = SourcePos_new("");
  src->e = ParseError_new();
  return src;
}

Source newFileSource(FILE* fp) {
  Source src = mem_malloc(sizeof(struct stSource));
  src->s = Stream_new(fp);
  src->pos = SourcePos_new("");
  src->e = ParseError_new();
  return src;
}

static char readChar(Source src, Ctx* ctx) {
  char c;
  if (!Stream_read((void*)&c, 1, src->s, ctx)) {
    ErrMsg m = {SysUnexpect, "end of input"};
    parseError(src, m);
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
    char c = readChar(src, &ctx);
    int tabWidth = 8;
    setSourcePos(src, SourcePos_advance1(tabWidth, c, getSourcePos(src)));
  }
  else {
    fprintf(stderr, "%s\n", ctx.msg);
    exit(1);
  }
}

intmax_t getSourceOffset(Source src) {
  Ctx ctx;
  TRY(&ctx) {
    return (intmax_t)Stream_getpos(src->s, &ctx);
  }
  else {
    fprintf(stderr, "%s\n", ctx.msg);
    exit(1);
  }
}

void setSourceOffset(Source src, intmax_t pos) {
  Ctx ctx;
  TRY(&ctx) {
    Stream_setpos((off_t)pos, src->s, &ctx);
  }
  else {
    fprintf(stderr, "%s\n", ctx.msg);
    exit(1);
  }
}

SourcePos getSourcePos(Source src) {
  return src->pos;
}

void setSourcePos(Source src, SourcePos pos) {
  src->pos = pos;
}
