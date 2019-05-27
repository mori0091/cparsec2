/* -*- coding:utf-8-unix -*- */

#include "cparsec2.h"

struct token_arg {
  int type;
  StringParser parser;
};

static Token Token_new(int type, const char* str) {
  Token tok = mem_malloc(sizeof(struct stToken));
  tok->type = type;
  tok->str = str;
  return tok;
}

static Token run_token(void* arg, Source src, Ctx* ex) {
  struct token_arg* self = (struct token_arg*)arg;
  Ctx ctx;
  TRY(&ctx) {
    parse(spaces, src, &ctx); /* skip whitespaces */
    const char* str = parse(self->parser, src, &ctx);
    return Token_new(self->type, str);
  }
  else {
    cthrow(ex, ctx.msg);
  }
}

TokenParser token_c(int type, char c) {
  return token(type, char1(c));
}

TokenParser token_s(int type, const char* s) {
  return token(type, string1(s));
}

TokenParser token_Char(int type, CharParser p) {
  return token_String(type, seq(p));
}

TokenParser token_String(int type, StringParser p) {
  struct token_arg* arg = mem_malloc(sizeof(struct token_arg));
  arg->type = type;
  arg->parser = p;
  return genParser(run_token, arg);
}
