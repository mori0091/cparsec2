/* -*- coding: utf-8-unix -*- */

#include <cparsec2.h>

#include "LList.h"

typedef struct {
  const char* name; // identifier
  int length;       // length of the identifier
  int offset;       // offset address of the local variable
  int size;         // size of the local variable
} LVar;

TYPEDEF_LList(LVar, LVar);
DECLARE_LList(LVar);
DEFINE_LList(LVar);
#define llist_cons(x, xs) (GENERIC((xs), LList, LList_CONS, LVar)(x, xs))
#define llist_head(xs) (GENERIC((xs), LList, LList_HEAD, LVar)(xs))
#define llist_tail(xs) (GENERIC((xs), LList, LList_TAIL, LVar)(xs))

// linked-list of local variables
LList(LVar) locals = NULL;

int getLVarOffsetMax(void) {
  if (!locals) {
    return 0;
  } else {
    LVar x = llist_head(locals);
    return x.offset + x.size;
  }
}

// find local variable or register if not found
LVar findLVar(const char* name) {
  int length = list_length(name);
  for (LList(LVar) xs = locals; xs; xs = llist_tail(xs)) {
    LVar x = llist_head(xs);
    if (length == x.length && strncmp(name, x.name, length) == 0) {
      return x;
    }
  }
  LVar x = (LVar){.name = name,
                  .length = length,
                  .offset = getLVarOffsetMax(),
                  .size = 8};
  locals = llist_cons(x, locals);
  return x;
}

// program  = {stmt} endOfFile
// stmt     = expr ";" {expr ";"}
// expr     = assign
// assign   = equality {"=" assign}
// equality = relation {("==" | "!=") relation}
// relation = addsub {("<" | "<=" | ">" | ">=") addsub}
// addsub   = muldiv { ("+" | "-") muldiv }
// muldiv   = unary { ("*" | "/") unary }
// unary    = ["+" | "-"] term
// term     = number | "(" expr ")"
PARSER(List(Node)) program;
PARSER(Node) stmt;
PARSER(Node) expr;
PARSER(Node) assign;
PARSER(Node) equality;
PARSER(Node) relation;
PARSER(Node) addsub;
PARSER(Node) muldiv;
PARSER(Node) unary;
PARSER(Node) term;

PARSER(Char) identStart;
PARSER(Char) identLetter;
PARSER(String) ident;

PARSER(String) eq_neq;    // eq_neq = "==" | "!="
PARSER(Char) lt_gt;       // lt_gt = "<" | ">"
PARSER(Char) plus_minus;  // plus_minus = "+" | "-"
PARSER(Char) star_slash;  // star_slash = "*" | "/"
PARSER(Char) open_paren;  // open_paren = "("
PARSER(Char) close_paren; // close_paren = ")"

static Node assign_fn(void* arg, Source src, Ctx* ex) {
  PARSER(List(Node)) p = arg; /* equality {"=" equality} */
  List(Node) xs = parse(p, src, ex);
  Node* itr = list_end(xs) - 1;
  Node* end = list_begin(xs) - 1;
  if (itr == end) {
    cthrow(ex, error("no expression"));
  }
  Node x = *itr--;
  while (itr != end) {
    x = nd_assign(*itr--, x);
  }
  return x;
}

static Node equality_fn(void* arg, Source src, Ctx* ex) {
  UNUSED(arg);
  Node x = parse(relation, src, ex);
  Ctx ctx;
  TRY(&ctx) {
    for (;;) {
      const char* op = parse(eq_neq, src, &ctx);
      Node y = parse(relation, src, ex);
      if (op[0] == '=') {
        x = nd_EQ(x, y);
      } else {
        x = nd_NE(x, y);
      }
    }
  }
  return x;
}

static Node relation_fn(void* arg, Source src, Ctx* ex) {
  UNUSED(arg);
  Node x = parse(addsub, src, ex);
  Ctx ctx;
  TRY(&ctx) {
    for (;;) {
      char c = parse(lt_gt, src, &ctx);
      if (peek(src, ex) == '=') {
        consume(src);
        Node y = parse(addsub, src, ex);
        if (c == '<') {
          x = nd_LE(x, y);
        } else {
          x = nd_LE(y, x);
        }
      } else {
        Node y = parse(addsub, src, ex);
        if (c == '<') {
          x = nd_LT(x, y);
        } else {
          x = nd_LT(y, x);
        }
      }
    }
  }
  return x;
}

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
  TRY(&ctx) {
    const char* name = parse(ident, src, &ctx);
    LVar lvar = findLVar(name);
    return nd_lvar(lvar.offset, lvar.size);
  }
  return nd_number(parse(number, src, ex));
}

#define sepBy(sep, p) cons(p, many(skip1st(sep, p)))

void setup(void) {
  term = token(PARSER_GEN(Node)(term_fn, NULL));
  unary = token(PARSER_GEN(Node)(unary_fn, NULL));
  muldiv = token(PARSER_GEN(Node)(muldiv_fn, NULL));
  addsub = token(PARSER_GEN(Node)(addsub_fn, NULL));
  relation = token(PARSER_GEN(Node)(relation_fn, NULL));
  equality = token(PARSER_GEN(Node)(equality_fn, NULL));
  assign = token(
      PARSER_GEN(Node)(assign_fn, sepBy(token(char1('=')), equality)));
  expr = assign;
  stmt = skip2nd(expr, token(char1(';')));
  program = skip2nd(many(stmt), token(endOfFile));

  identStart = either(char1('_'), alpha);
  identLetter = either(char1('_'), alnum);
  ident = cons(identStart, many(identLetter));

  eq_neq = token(tryp(either("!=", "==")));
  lt_gt = token(either((char)'<', (char)'>'));
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
  //   allocate local variables
  fprintf(stdout, "  push rbp\n");
  fprintf(stdout, "  mov rbp, rsp\n");
  fprintf(stdout, "  sub rsp, %d\n", getLVarOffsetMax());
}

static void codegen_epilogue(void) {
  // - epilogue
  //   deallocate local variables
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
    List(Node) ast = parse(program, src, &ctx);

    // [generate assembly code]
    codegen_header();
    {
      codegen_prologue();
      {
        Node* itr = list_begin(ast);
        Node* end = list_end(ast);
        while (itr != end) {
          // generate code of the statement.
          codegen(*itr++, stdout);
          // pop result of the last expression of the statement.
          fprintf(stdout, "  pop rax\n");
        }
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
