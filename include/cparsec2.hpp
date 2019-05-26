/* -*- coding:utf-8-unix -*- */
#pragma once

#include <cparsec2.h>
#include <string>

#ifdef parse
#undef parse
#endif

#ifdef parseTest
#undef parseTest
#define parseTest(p, input)                                              \
  static_assert(0, "parseTest(p, input) is not available yet in C++")
#endif

inline char parse(CharParser p, Source src) {
  Ctx ctx;
  TRY(&ctx) { return parse_Char(p, src, &ctx); }
  else {
    std::string ex(ctx.msg);
    throw ex;
  }
}

inline std::string parse(StringParser p, Source src) {
  Ctx ctx;
  TRY(&ctx) { return parse_String(p, src, &ctx); }
  else {
    std::string ex(ctx.msg);
    throw ex;
  }
}

inline Token parse(TokenParser p, Source src) {
  Ctx ctx;
  TRY(&ctx) { return parse_Token(p, src, &ctx); }
  else {
    std::string ex(ctx.msg);
    throw ex;
  }
}
