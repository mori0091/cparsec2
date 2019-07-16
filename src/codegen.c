/* -*- coding:utf-8-unix -*- */

#include <cparsec2.h>

Node Node_new(NodeFn f, void* arg) {
  Node node = mem_malloc(sizeof(struct NodeSt));
  node->run = f;
  node->arg = arg;
  return node;
}

void codegen(Node node, FILE* out) {
  node->run(node->arg, out);
}

static void run_nd_number(void* arg, FILE* out) {
  fprintf(out, "  push %d\n", (int)(intptr_t)arg);
}

Node nd_number(int val) {
  return Node_new(run_nd_number, (void*)(intptr_t)val);
}

static void codegen_lval(uint8_t name, FILE* out) {
  int offset = ('z' - name + 1) * 8;
  fprintf(out, "  mov rax, rbp\n");
  fprintf(out, "  sub rax, %d\n", offset);
  fprintf(out, "  push rax\n");
}

static void run_nd_ident(void* arg, FILE* out) {
  codegen_lval((uint8_t)(uintptr_t)arg, out);
  fprintf(out, "  pop rax\n");
  fprintf(out, "  mov rax, [rax]\n");
  fprintf(out, "  push rax\n");
}

Node nd_ident(uint8_t name) {
  return Node_new(run_nd_ident, (void*)(uintptr_t)name);
}

static void run_nd_assign(void* arg, FILE* out) {
  Node* node = (Node*)arg;
  if (node[0]->run != run_nd_ident) {
    fprintf(stderr, "lvalue of assignment is not a variable");
    exit(1);
  }
  codegen_lval((uint8_t)(uintptr_t)node[0]->arg, out);
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