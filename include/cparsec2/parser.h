/* -*- coding:utf-8-unix -*- */
#pragma once

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

// ---- resource management ----
#include "alloc.h"
// ---- error and exception handling ----
#include "exception.h"
// ---- source of input character sequence ----
#include "source.h"

// ---- common macros ----
#include "macros.h"
// ---- code generators as abstract syntax tree ----
#include "codegen.h"
// ---- type generic container ----
#include "container.h"
// ---- None type and NONE value ----
#include "none.h"

#define RETURN_TYPE(T) CAT(T, _return_type)

#define PARSER(T) CAT(T, Parser)
#define PARSER_ST(T) CAT(PARSER(T), St)
#define PARSER_FN(T) CAT(PARSER(T), Fn)
#define PARSER_GEN(T) CAT(PARSER(T), _gen)
#define PARSE(T) CAT(PARSER(T), _parse)
#define PARSETEST(T) CAT(PARSER(T), _parseTest)
#define PARSER_ID_FN(T) CAT(PARSER(T), _identity)
#define SHOW(T) CAT(T, _show)

#define TYPEDEF_PARSER(T, R)                                             \
  typedef struct PARSER_ST(T) * PARSER(T);                               \
  typedef R RETURN_TYPE(PARSER(T));                                      \
  typedef R (*PARSER_FN(T))(void* arg, Source src, Ctx* ex)

#define DECLARE_PARSER(T)                                                \
  PARSER(T) PARSER_GEN(T)(PARSER_FN(T) f, void* arg);                    \
  RETURN_TYPE(PARSER(T)) PARSE(T)(PARSER(T) p, Source src, Ctx * ctx);   \
  bool PARSETEST(T)(const char* msg, PARSER(T) p, const char* input);    \
  PARSER(T) PARSER_ID_FN(T)(PARSER(T) p);                                \
  void SHOW(T)(RETURN_TYPE(PARSER(T)) val);                              \
  END_OF_STATEMENTS

#define DEFINE_PARSER(T)                                                 \
  struct PARSER_ST(T) {                                                  \
    PARSER_FN(T) run;                                                    \
    void* arg;                                                           \
  };                                                                     \
  PARSER(T) PARSER_GEN(T)(PARSER_FN(T) f, void* arg) {                   \
    PARSER(T) p = mem_malloc(sizeof(struct PARSER_ST(T)));               \
    p->run = f;                                                          \
    p->arg = arg;                                                        \
    return p;                                                            \
  }                                                                      \
  RETURN_TYPE(PARSER(T)) PARSE(T)(PARSER(T) p, Source src, Ctx * ctx) {  \
    assert(ctx);                                                         \
    return p->run(p->arg, src, ctx);                                     \
  }                                                                      \
  bool PARSETEST(T)(const char* msg, PARSER(T) p, const char* input) {   \
    printf("%s", msg);                                                   \
    Source src = Source_new(input);                                      \
    Ctx ctx;                                                             \
    TRY(&ctx) {                                                          \
      SHOW(T)(PARSE(T)(p, src, &ctx));                                   \
      printf("\n");                                                      \
      return true;                                                       \
    }                                                                    \
    else {                                                               \
      printf("error:%s\n", ctx.msg);                                     \
      mem_free((void*)ctx.msg);                                          \
      printf("%s\n", getParseErrorAsString(src));                        \
      return false;                                                      \
    }                                                                    \
  }                                                                      \
  PARSER(T) PARSER_ID_FN(T)(PARSER(T) p) {                               \
    return p;                                                            \
  }                                                                      \
  /* SHOW(T)(RETURN_TYPE(PARSER(T) x) must be defined explicitly */      \
  void SHOW(T)(RETURN_TYPE(PARSER(T)) x);                                \
  END_OF_STATEMENTS

#ifdef __cplusplus
extern "C" {
#endif

// ---- NoneParser ----
TYPEDEF_PARSER(None, None);
// ---- CharParser ----
TYPEDEF_PARSER(Char, char);
// ---- StringParser ----
TYPEDEF_PARSER(String, const char*);
// ---- IntParser ----
TYPEDEF_PARSER(Int, int);

// ---- NoneListParser ----
TYPEDEF_PARSER(List(None), List(None));
// ---- StringListParser ----
TYPEDEF_PARSER(List(String), List(String));
// ---- IntListParser ----
TYPEDEF_PARSER(List(Int), List(Int));
// ---- NodeParser ----
TYPEDEF_PARSER(Node, Node);
// ---- NodeListParser ----
TYPEDEF_PARSER(List(Node), List(Node));

FOREACH(DECLARE_PARSER, TYPESET(1));

#ifdef __cplusplus
}
#endif
