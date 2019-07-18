/* -*- coding:utf-8-unix -*- */
#pragma once

#include <stdint.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*NodeFn)(void* arg, FILE* out);
typedef struct NodeSt* Node;
struct NodeSt {
  NodeFn run;
  void* arg;
};

Node Node_new(NodeFn f, void* arg);
void codegen(Node node, FILE* out);

Node nd_number(int val);
Node nd_lvar(int offset, int size);
Node nd_stmt(Node expr);
Node nd_return(Node expr);
Node nd_c_for(Node* xs);
Node nd_assign(Node lhs, Node rhs);
Node nd_EQ(Node lhs, Node rhs);
Node nd_NE(Node lhs, Node rhs);
Node nd_LT(Node lhs, Node rhs);
Node nd_LE(Node lhs, Node rhs);
Node nd_add(Node lhs, Node rhs);
Node nd_sub(Node lhs, Node rhs);
Node nd_mul(Node lhs, Node rhs);
Node nd_div(Node lhs, Node rhs);

#ifdef __cplusplus
}
#endif
