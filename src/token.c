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

PARSER(Token) token_c(int type, char c) {
  return token(type, char1(c));
}

PARSER(Token) token_s(int type, const char* s) {
  return token(type, string1(s));
}

PARSER(Token) token_Char(int type, PARSER(Char) p) {
  return token_String(type, seq(p));
}

PARSER(Token) token_String(int type, PARSER(String) p) {
  struct token_arg* arg = mem_malloc(sizeof(struct token_arg));
  arg->type = type;
  arg->parser = p;
  return PARSER_GEN(Token)(run_token, arg);
}
