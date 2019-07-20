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

TYPEDEF_LList(String, const char*);
DECLARE_LList(String);
DEFINE_LList(String);

#define llist_cons(x, xs)                                                \
  (GENERIC((xs), LList, LList_CONS, LVar, String)(x, xs))
#define llist_head(xs)                                                   \
  (GENERIC((xs), LList, LList_HEAD, LVar, String)(xs))
#define llist_tail(xs)                                                   \
  (GENERIC((xs), LList, LList_TAIL, LVar, String)(xs))

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

LList(String) keywords = NULL;

bool is_keyword(const char* s) {
  for (LList(String) xs = keywords; xs; xs = llist_tail(xs)) {
    if (!strcmp(llist_head(xs), s)) {
      return true;
    }
  }
  return false;
}

void add_keyword(const char* s) {
  if (!is_keyword(s)) {
    keywords = llist_cons(s, keywords);
  }
}

#define sepBy(sep, p) cons(p, many(skip1st(sep, p)))
#define between(open, close, p) skip1st(open, skip2nd(p, close))
#define parens(p) between(open_paren, close_paren, p)

static PARSER(String) keyword(const char* word);

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
    if (is_keyword(name)) {
      cthrow(ex, error("expected identifier or '(' but was '%s'", name));
    }
    LVar lvar = findLVar(name);
    return nd_lvar(lvar.offset, lvar.size);
  }
  return nd_number(parse(number, src, ex));
}

static Node stmt_fn(void* arg, Source src, Ctx* ex) {
  PARSER(Node) p = arg;
  return nd_stmt(parse(p, src, ex));
}

static Node return_fn(void* arg, Source src, Ctx* ex) {
  PARSER(Node) p = arg;
  return nd_return(parse(p, src, ex));
}

static const char* keyword_fn(void* arg, Source src, Ctx* ex) {
  PARSER(String) p = arg;
  const char* s = parse(p, src, ex);
  Ctx ctx;
  TRY(&ctx) {
    char c = peek(src, &ctx);
    if (c == '_' || is_alnum(c)) {
      cthrow(ex, error("unexpected \"%s%c\"", s, (int)c));
    }
  }
  else {
    mem_free((void*)ctx.msg);
  }
  return s;
}

static PARSER(String) keyword(const char* word) {
  add_keyword(word);
  return PARSER_GEN(String)(keyword_fn, token(string1(word)));
}

static Node c_while_fn(void* arg, Source src, Ctx* ex) {
  // p = "while (cond)" -> cond
  PARSER(Node) p = arg;
  Node cond = parse(p, src, ex);
  Node body = parse(stmt, src, ex);
  Buff(Node) b = {0};
  buff_push(&b, NULL); // init
  buff_push(&b, cond);
  buff_push(&b, NULL); // next
  buff_push(&b, body);
  return nd_c_for(list_begin(buff_finish(&b)));
}

static Node c_for_fn(void* arg, Source src, Ctx* ex) {
  // ps = "for (init; cond; next)" -> [init, cond, next]
  PARSER(List(Node)) ps = arg;
  Buff(Node) b = {0};
  buff_append(&b, parse(ps, src, ex)); // [init, cond, next]
  buff_push(&b, parse(stmt, src, ex)); // [init, cond, next, body]
  return nd_c_for(list_begin(buff_finish(&b)));
}

void setup(void) {
  term = token(PARSER_GEN(Node)(term_fn, NULL));
  identStart = either(char1('_'), alpha);
  identLetter = either(char1('_'), alnum);
  ident = cons(identStart, many(identLetter));

  eq_neq = token(tryp(either("!=", "==")));
  lt_gt = token(either((char)'<', (char)'>'));
  plus_minus = token(either((char)'+', (char)'-'));
  star_slash = token(either((char)'*', (char)'/'));
  open_paren = token(char1('('));
  close_paren = token(char1(')'));

  unary = token(PARSER_GEN(Node)(unary_fn, NULL));
  muldiv = PARSER_GEN(Node)(muldiv_fn, NULL);
  addsub = PARSER_GEN(Node)(addsub_fn, NULL);
  relation = PARSER_GEN(Node)(relation_fn, NULL);
  equality = PARSER_GEN(Node)(equality_fn, NULL);

  PARSER(List(Node)) assign_expr = sepBy(token(char1('=')), equality);
  assign = PARSER_GEN(Node)(assign_fn, assign_expr);
  expr = assign;

  PARSER(Node)
  expr_stmt = PARSER_GEN(Node)(stmt_fn, skip2nd(expr, token(char1(';'))));

  PARSER(Node)
  return_stmt =
      PARSER_GEN(Node)(return_fn, skip1st(keyword("return"), expr_stmt));

  PARSER(Node)
  c_for_stmt = PARSER_GEN(Node)(
      c_for_fn,
      skip1st(keyword("for"), parens(seq(expr_stmt, expr_stmt, expr))));

  PARSER(Node)
  c_while_stmt = PARSER_GEN(Node)(
      c_while_fn, skip1st(keyword("while"), parens(expr)));

  stmt = FOLDL(either, tryp(return_stmt), tryp(c_for_stmt),
               tryp(c_while_stmt), expr_stmt);

  program = skip2nd(many1(stmt), token(endOfFile));
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
    fprintf(stderr, "Usage: excc <expr>\n");
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
