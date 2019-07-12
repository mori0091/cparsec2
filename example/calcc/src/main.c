/* -*- coding: utf-8-unix -*- */

#include <cparsec2.h>

PARSER(Node) expr;   // expr   = addsub
PARSER(Node) addsub; // addsub = muldiv { ("+" | "-") muldiv }
PARSER(Node) muldiv; // muldiv = unary { ("*" | "/") unary }
PARSER(Node) unary;  // unary  = ["+" | "-"] term
PARSER(Node) term;   // term   = number | "(" expr ")"

PARSER(Char) plus_minus;  // plus_minus = "+" | "-"
PARSER(Char) star_slash;  // star_slash = "*" | "/"
PARSER(Char) open_paren;  // open_paren = "("
PARSER(Char) close_paren; // close_paren = ")"

static Node addsub_fn(void* arg, Source src, Ctx* ex) {
  UNUSED(arg);
  Node x = parse(muldiv, src, ex);
  Ctx ctx;
  TRY(&ctx) {
    while (peek(src, &ctx)) {
      char op = parse(plus_minus, src, &ctx);
      Node y = parse(muldiv, src, ex);
      if (op == '+') {
        x = nd_add(x, y);
      } else {
        x = nd_sub(x, y);
      }
    }
  }
  return x;
}

static Node muldiv_fn(void* arg, Source src, Ctx* ex) {
  UNUSED(arg);
  Node x = parse(unary, src, ex);
  Ctx ctx;
  TRY(&ctx) {
    while (peek(src, &ctx)) {
      char op = parse(star_slash, src, &ctx);
      Node y = parse(unary, src, ex);
      if (op == '*') {
        x = nd_mul(x, y);
      } else {
        x = nd_div(x, y);
      }
    }
  }
  return x;
}

static Node unary_fn(void* arg, Source src, Ctx* ex) {
  UNUSED(arg);
  if ('+' == peek(src, ex)) {
    consume(src);
    return parse(term, src, ex);
  }
  if ('-' == peek(src, ex)) {
    consume(src);
    return nd_sub(nd_number(0), parse(term, src, ex));
  }
  return parse(term, src, ex);
}

static Node term_fn(void* arg, Source src, Ctx* ex) {
  UNUSED(arg);
  Ctx ctx;
  TRY(&ctx) {
    parse(open_paren, src, &ctx);
    Node x = parse(expr, src, ex);
    parse(close_paren, src, ex);
    return x;
  }
  else {
    return nd_number(parse(number, src, ex));
  }
}

void setup(void) {
  term = token(PARSER_GEN(Node)(term_fn, NULL));
  unary = token(PARSER_GEN(Node)(unary_fn, NULL));
  muldiv = token(PARSER_GEN(Node)(muldiv_fn, NULL));
  addsub = token(PARSER_GEN(Node)(addsub_fn, NULL));
  expr = addsub;

  plus_minus = token(either((char)'+', (char)'-'));
  star_slash = token(either((char)'*', (char)'/'));
  open_paren = token(char1('('));
  close_paren = token(char1(')'));
}

static void codegen_header(void) {
  // - header
  fprintf(stdout, ".intel_syntax noprefix\n");
  fprintf(stdout, ".global main\n");
  fprintf(stdout, "main:\n");
}

static void codegen_footer(void) {
  // - footer
  //   note: 'rax' has already the result of the last expression,
  //   and the value of 'rax' shall be the return value.
  fprintf(stdout, "  ret\n");
}

static void codegen_prologue(void) {
  // - prologue
  //   allocate 26 local variables 'a'-'z'
  fprintf(stdout, "  push rbp\n");
  fprintf(stdout, "  mov rbp, rsp\n");
  fprintf(stdout, "  sub rsp, 208\n"); // 208 bytes = 8 bytes * 26
}

static void codegen_epilogue(void) {
  // - epilogue
  //   deallocate 26 local variables 'a'-'z'
  fprintf(stdout, "  mov rsp, rbp\n");
  fprintf(stdout, "  pop rbp\n");
}

int main(int argc, char** argv) {
  if (argc < 2) {
    fprintf(stderr, "Usage: calcc <expr>\n");
    return 0;
  }

  cparsec2_init();

  setup();

  Ctx ctx;
  TRY(&ctx) {
    Source src = Source_new(argv[1]);
    Node ast = parse(skip2nd(expr, token(endOfFile)), src, &ctx);

    // [generate assembly code]
    codegen_header();
    {
      codegen_prologue();
      {
        // generate code of the statement.
        codegen(ast, stdout);
        // pop result of the last expression of the statement.
        fprintf(stdout, "  pop rax\n");
      }
      codegen_epilogue();
    }
    codegen_footer();

    return 0;
  }
  else {
    fprintf(stderr, "error:%s\n", ctx.msg);
    return 1;
  }
}
