/* -*- coding: utf-8-unix -*- */

#include <cparsec2.h>

PARSER(Int) expr;   // expr   = addsub
PARSER(Int) addsub; // addsub = muldiv { ("+" | "-") muldiv }
PARSER(Int) muldiv; // muldiv = unary { ("*" | "/") unary }
PARSER(Int) unary;  // unary  = ["+" | "-"] term
PARSER(Int) term;   // term   = number | "(" expr ")"

PARSER(Char) plus_minus;  // plus_minus = "+" | "-"
PARSER(Char) star_slash;  // star_slash = "*" | "/"
PARSER(Char) open_paren;  // open_paren = "("
PARSER(Char) close_paren; // close_paren = ")"

static int addsub_fn(void* arg, Source src, Ctx* ex) {
  UNUSED(arg);
  int x = parse(muldiv, src, ex);
  Ctx ctx;
  TRY(&ctx) {
    while (peek(src, &ctx)) {
      char op = parse(plus_minus, src, &ctx);
      int y = parse(muldiv, src, ex);
      if (op == '+') {
        x += y;
      } else {
        x -= y;
      }
    }
  }
  return x;
}

static int muldiv_fn(void* arg, Source src, Ctx* ex) {
  UNUSED(arg);
  int x = parse(unary, src, ex);
  Ctx ctx;
  TRY(&ctx) {
    while (peek(src, &ctx)) {
      char op = parse(star_slash, src, &ctx);
      int y = parse(unary, src, ex);
      if (op == '*') {
        x *= y;
      } else {
        x /= y;
      }
    }
  }
  return x;
}

static int unary_fn(void* arg, Source src, Ctx* ex) {
  UNUSED(arg);
  if ('+' == peek(src, ex)) {
    consume(src);
    return parse(term, src, ex);
  }
  if ('-' == peek(src, ex)) {
    consume(src);
    return -(parse(term, src, ex));
  }
  return parse(term, src, ex);
}

static int term_fn(void* arg, Source src, Ctx* ex) {
  UNUSED(arg);
  Ctx ctx;
  TRY(&ctx) {
    parse(open_paren, src, &ctx);
    int x = parse(expr, src, ex);
    parse(close_paren, src, ex);
    return x;
  }
  else {
    return parse(number, src, ex);
  }
}

void setup(void) {
  term = token(PARSER_GEN(Int)(term_fn, NULL));
  unary = token(PARSER_GEN(Int)(unary_fn, NULL));
  muldiv = token(PARSER_GEN(Int)(muldiv_fn, NULL));
  addsub = token(PARSER_GEN(Int)(addsub_fn, NULL));
  expr = addsub;

  plus_minus = token(either((char)'+', (char)'-'));
  star_slash = token(either((char)'*', (char)'/'));
  open_paren = token(char1('('));
  close_paren = token(char1(')'));
}

int main(int argc, char** argv) {
  if (argc < 2) {
    fprintf(stderr, "Usage: calc <expr>\n");
    return 0;
  }

  cparsec2_init();

  setup();
  PARSER(Int) p = skip1st(spaces, skip2nd(expr, endOfFile));
  return !parseTest(p, argv[1]);
}
