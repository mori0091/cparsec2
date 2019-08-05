/* -*- coding:utf-8-unix -*- */

#include <cparsec2.h>

Node Node_new(NodeFn f, void* arg) {
  Node node = mem_malloc(sizeof(struct NodeSt));
  node->run = f;
  node->arg = arg;
  return node;
}

void codegen(Node node, FILE* out) {
  assert(out);
  if (node) {
    node->run(node->arg, out);
  }
}

static void run_nd_number(void* arg, FILE* out) {
  fprintf(out, "  push %d\n", (int)(intptr_t)arg);
}

Node nd_number(int val) {
  return Node_new(run_nd_number, (void*)(intptr_t)val);
}

static void codegen_lval(int offset, int size, FILE* out) {
  fprintf(out, "  mov rax, rbp\n");
  fprintf(out, "  sub rax, %d\n", offset + size);
  fprintf(out, "  push rax\n");
}

static void run_nd_lvar(void* arg, FILE* out) {
  int* lvar = arg;
  int offset = lvar[0];
  int size = lvar[1];
  codegen_lval(offset, size, out);
  fprintf(out, "  pop rax\n");
  fprintf(out, "  mov rax, [rax]\n");
  fprintf(out, "  push rax\n");
}

Node nd_lvar(int offset, int size) {
  int* lvar = mem_malloc(sizeof(int) * 2);
  lvar[0] = offset;
  lvar[1] = size;
  return Node_new(run_nd_lvar, (void*)lvar);
}

struct nd_c_call_arg {
  const char* name;
  int argc;
  Node* argv;
};

static const char* c_call_registers[] = {
    "rax", // return value
    "rdi", // argument #1
    "rsi", // argument #2
    "rdx", // argument #3
    "rcx", // argument #4
    "r8" , // argument #5
    "r9" , // argument #6
};

static void run_nd_c_call(void* arg, FILE* out) {
  struct nd_c_call_arg* p = arg;
  assert(p && p->name && 0 <= p->argc);
  for (int i = p->argc; 0 < i; --i) {
    codegen(p->argv[i-1], out);
  }
  int n = (p->argc > 6) ? 6 : p->argc;
  for (int i = 1; i <= n; ++i) {
    fprintf(out, "  pop %s\n", c_call_registers[i]);
  }
  fprintf(out, "  call %s\n", p->name);
  fprintf(out, "  push rax\n");
}

Node nd_c_call(const char* name, int argc, Node* argv) {
  struct nd_c_call_arg* p = mem_malloc(sizeof(struct nd_c_call_arg));
  p->name = name;
  p->argc = argc;
  p->argv = argv;
  return Node_new(run_nd_c_call, p);
}

static void run_nd_stmt(void* arg, FILE* out) {
  Node expr = arg;
  if (expr) {
    codegen(expr, out);
    fprintf(out, "  pop rax\n");
  }
}

Node nd_stmt(Node expr) {
  return Node_new(run_nd_stmt, expr);
}

static void run_nd_return(void* arg, FILE* out) {
  codegen((Node)arg, out);
  // - epilogue
  //   deallocate local variables
  fprintf(out, "  mov rsp, rbp\n");
  fprintf(out, "  pop rbp\n");
  // - footer
  //   note: 'rax' has already the result of the last expression,
  //   and the value of 'rax' shall be the return value.
  fprintf(out, "  ret\n");
}

Node nd_return(Node expr) {
  return Node_new(run_nd_return, expr);
}

static int counter = 0;

static void run_nd_c_for(void* arg, FILE* out) {
  Node* node = arg;
  int label = ++counter;
  if (node[0]) {
    codegen(node[0], out); // init (expr)
    fprintf(out, "  pop rax\n");
  }
  fprintf(out, ".Lbegin%d:\n", label);
  if (node[1]) {
    codegen(node[1], out); // cond (expr)
    fprintf(out, "  pop rax\n");
    fprintf(out, "  cmp rax, 0\n");
    fprintf(out, "  je  .Lend%d\n", label);
  }
  codegen(node[3], out); // body (stmt)
  if (node[2]) {
    codegen(node[2], out); // next (expr)
    fprintf(out, "  pop rax\n");
  }
  fprintf(out, "  jmp .Lbegin%d\n", label);
  fprintf(out, ".Lend%d:\n", label);
}

Node nd_c_for(Node init, Node cond, Node next, Node body) {
  Node* arg = mem_malloc(sizeof(Node) * 4);
  arg[0] = init;
  arg[1] = cond;
  arg[2] = next;
  arg[3] = body;
  return Node_new(run_nd_c_for, arg);
}

static void run_nd_c_if_else(void* arg, FILE* out) {
  Node* node = arg;
  int label = ++counter;
  codegen(node[0], out); // cond (expr)
  fprintf(out, "  pop rax\n");
  fprintf(out, "  cmp rax, 0\n");
  fprintf(out, "  je  .Lelse%d\n", label);
  codegen(node[1], out); // then (stmt)
  fprintf(out, "  jmp .Lend%d\n", label);
  fprintf(out, ".Lelse%d:\n", label);
  codegen(node[2], out); // else (stmt)
  fprintf(out, ".Lend%d:\n", label);
}

Node nd_c_if_else(Node cond, Node then_body, Node else_body) {
  Node* arg = mem_malloc(sizeof(Node) * 3);
  arg[0] = cond;
  arg[1] = then_body;
  arg[2] = else_body;
  return Node_new(run_nd_c_if_else, arg);
}

static void run_nd_c_compound_stmt(void* arg, FILE* out) {
  Node** xs = arg;
  Node* itr = xs[0];
  Node* end = xs[1];
  while (itr != end) {
    codegen(*itr++, out);
  }
}

Node nd_c_compound_stmt(int n, Node* block) {
  Node** arg = mem_malloc(sizeof(Node*)*2);
  arg[0] = block;
  arg[1] = block + n;
  return Node_new(run_nd_c_compound_stmt, arg);
}

static void run_nd_assign(void* arg, FILE* out) {
  Node* node = (Node*)arg;
  if (node[0]->run != run_nd_lvar) {
    fprintf(stderr, "error:lvalue of assignment is not a variable");
    exit(1);
  }
  int* lvar = node[0]->arg;
  int offset = lvar[0];
  int size = lvar[1];
  codegen_lval(offset, size, out);
  codegen(node[1], out);
  fprintf(out, "  pop rdi\n");
  fprintf(out, "  pop rax\n");
  fprintf(out, "  mov [rax], rdi\n");
  fprintf(out, "  push rdi\n");
}

Node nd_assign(Node lhs, Node rhs) {
  Node* arg = mem_malloc(sizeof(struct NodeSt) * 2);
  arg[0] = lhs;
  arg[1] = rhs;
  return Node_new(run_nd_assign, (void*)arg);
}

struct infixArg {
  void (*f)(FILE* out);
  Node lhs;
  Node rhs;
};

static void run_infix(void* arg, FILE* out) {
  struct infixArg* p = arg;
  codegen(p->lhs, out);
  codegen(p->rhs, out);
  fprintf(out, "  pop rdi\n");
  fprintf(out, "  pop rax\n");
  p->f(out);
  fprintf(out, "  push rax\n");
}

static Node infix(void (*f)(FILE* out), Node lhs, Node rhs) {
  struct infixArg* arg = mem_malloc(sizeof(struct infixArg));
  arg->f = f;
  arg->lhs = lhs;
  arg->rhs = rhs;
  return Node_new(run_infix, (void*)arg);
}

static void run_nd_EQ(FILE* out) {
  fprintf(out, "  cmp rax, rdi\n");
  fprintf(out, "  sete al\n");
  fprintf(out, "  movzb rax, al\n");
}
static void run_nd_NE(FILE* out) {
  fprintf(out, "  cmp rax, rdi\n");
  fprintf(out, "  setne al\n");
  fprintf(out, "  movzb rax, al\n");
}
static void run_nd_LT(FILE* out) {
  fprintf(out, "  cmp rax, rdi\n");
  fprintf(out, "  setl al\n");
  fprintf(out, "  movzb rax, al\n");
}
static void run_nd_LE(FILE* out) {
  fprintf(out, "  cmp rax, rdi\n");
  fprintf(out, "  setle al\n");
  fprintf(out, "  movzb rax, al\n");
}
static void run_nd_add(FILE* out) {
  fprintf(out, "  add rax, rdi\n");
}
static void run_nd_sub(FILE* out) {
  fprintf(out, "  sub rax, rdi\n");
}
static void run_nd_mul(FILE* out) {
  fprintf(out, "  mul rdi\n");
}
static void run_nd_div(FILE* out) {
  fprintf(out, "  mov rdx, 0\n");
  fprintf(out, "  div rdi\n");
}

Node nd_EQ(Node lhs, Node rhs) {
  return infix(run_nd_EQ, lhs, rhs);
}
Node nd_NE(Node lhs, Node rhs) {
  return infix(run_nd_NE, lhs, rhs);
}
Node nd_LT(Node lhs, Node rhs) {
  return infix(run_nd_LT, lhs, rhs);
}
Node nd_LE(Node lhs, Node rhs) {
  return infix(run_nd_LE, lhs, rhs);
}
Node nd_GT(Node lhs, Node rhs) {
  return infix(run_nd_LT, rhs, lhs);
}
Node nd_GE(Node lhs, Node rhs) {
  return infix(run_nd_LE, rhs, lhs);
}
Node nd_add(Node lhs, Node rhs) {
  return infix(run_nd_add, lhs, rhs);
}
Node nd_sub(Node lhs, Node rhs) {
  return infix(run_nd_sub, lhs, rhs);
}
Node nd_mul(Node lhs, Node rhs) {
  return infix(run_nd_mul, lhs, rhs);
}
Node nd_div(Node lhs, Node rhs) {
  return infix(run_nd_div, lhs, rhs);
}
