/* -*- coding:utf-8-unix -*- */

#include "cparsec2.h"

struct token_arg {
  int type;
  PARSER(String) parser;
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

PARSER(Token) TOKEN(Char)(int type, PARSER(Char) p) {
  return token(type, seq(p));
}

PARSER(Token) TOKEN(String)(int type, PARSER(String) p) {
  struct token_arg* arg = mem_malloc(sizeof(struct token_arg));
  arg->type = type;
  arg->parser = p;
  return PARSER_GEN(Token)(run_token, arg);
}
