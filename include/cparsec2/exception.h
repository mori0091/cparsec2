/* -*- coding:utf-8-unix -*- */
#pragma once

#include <setjmp.h>

#ifdef __cplusplus
#define NORETURN [[noreturn]]
#else
#define NORETURN _Noreturn
#endif

#ifdef __cplusplus
extern "C" {
#endif

// context object for exception handling
typedef struct {
  const char* msg;
  jmp_buf e;
} Ctx;

// TRY(Ctx *ctx) {statement...} else {exception-handler...}
#define TRY(ctx) if (!setjmp((ctx)->e))

// throw exception
NORETURN void cthrow(Ctx* ctx, const char* msg);

#ifdef __cplusplus
}
#endif
