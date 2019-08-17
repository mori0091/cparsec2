/* -*- coding:utf-8-unix -*- */

#include <cparsec2/exception.h>

// ---- error and exception handling ----

_Noreturn void cthrow(Ctx* ctx, const char* msg) {
  ctx->msg = msg;
  longjmp(ctx->e, -1);
}
