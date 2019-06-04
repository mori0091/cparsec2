/* -*- coding: utf-8-unix -*- */

#include <cparsec2.h>

PARSER(Int) expr;   // expr   = addsub
PARSER(Int) addsub; // addsub = muldiv { ("+" | "-") muldiv }
PARSER(Int) muldiv; // muldiv = unary { ("*" | "/") unary }
PARSER(Int) unary;  // unary  = ["+" | "-"] term
PARSER(Int) term;   // term   = number | "(" expr ")"
PARSER(Int) number; // number = digits

PARSER(String) digits;   // digits = digit { digit }
PARSER(Char) plus_minus; // plus_minus = "+" | "-"
PARSER(Char) star_slash; // star_slash = "*" | "/"

static int addsub_fn(void* arg, Source src, Ctx* ex) {
  UNUSED(arg);
  int x = parse(muldiv, src, ex);
  Ctx ctx;
  TRY(&ctx) {
    while (peek(src, &ctx)) {
      parse(spaces, src, ex);
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
      parse(spaces, src, ex);
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
  parse(spaces, src, ex);
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
  parse(spaces, src, ex);
  if ((char)'(' == peek(src, ex)) {
    consume(src);
    parse(spaces, src, ex);
    int x = parse(expr, src, ex);
    parse(spaces, src, ex);
    parse(char1(')'), src, ex);
    return x;
  }
  return parse(number, src, ex);
}

static int number_fn(void* arg, Source src, Ctx* ex) {
  UNUSED(arg);
  parse(spaces, src, ex);
  return atoi(parse(digits, src, ex));
}

void setup(void) {
  digits = many1(digit);
  number = PARSER_GEN(Int)(number_fn, NULL);
  term = PARSER_GEN(Int)(term_fn, NULL);
  unary = PARSER_GEN(Int)(unary_fn, NULL);
  muldiv = PARSER_GEN(Int)(muldiv_fn, NULL);
  addsub = PARSER_GEN(Int)(addsub_fn, NULL);
  expr = addsub;

  plus_minus = either((char)'+', (char)'-');
  star_slash = either((char)'*', (char)'/');
}

int main(int argc, char** argv) {
  if (argc < 2) {
    fprintf(stderr, "Usage: calc <expr>\n");
    return 0;
  }

  cparsec2_init();

  setup();
  parseTest(expr, argv[1]);

  return 0;
}
