/* -*- coding:utf-8-unix -*- */

#include <string.h>

#include <cparsec2.h>

#define EXPECTS_FN(T) CAT(run_expects, T)
#define DEFINE_EXPECTS(T)                                                \
  static RETURN_TYPE(PARSER(T))                                          \
      EXPECTS_FN(T)(void* arg, Source src, Ctx* ex) {                    \
    void** ps = (void**)arg;                                             \
    const char* desc = ps[0];                                            \
    PARSER(T) p = ps[1];                                                 \
    Ctx ctx;                                                             \
    TRY(&ctx) {                                                          \
      return parse(p, src, &ctx);                                        \
    }                                                                    \
    else {                                                               \
      ErrMsg m = {Expect, desc};                                         \
      parseError(src, m);                                                \
      const char* bw = strstr(ctx.msg, "but was");                       \
      if (bw) {                                                          \
        const char* msg = error("expects %s %s", desc, bw);              \
        mem_free((void*)ctx.msg);                                        \
        cthrow(ex, msg);                                                 \
      } else {                                                           \
        cthrow(ex, ctx.msg);                                             \
      }                                                                  \
    }                                                                    \
  }                                                                      \
  PARSER(T) EXPECTS(T)(const char* desc, PARSER(T) p) {                  \
    void** ps = mem_malloc(sizeof(void*) * 2);                           \
    ps[0] = (void*)desc;                                                 \
    ps[1] = (void*)p;                                                    \
    return PARSER_GEN(T)(EXPECTS_FN(T), ps);                             \
  }                                                                      \
  END_OF_STATEMENTS

FOREACH(DEFINE_EXPECTS, TYPESET(1));
