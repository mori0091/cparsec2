/* -*- coding:utf-8-unix -*- */
#pragma once

#include <cparsec2.h>
#include <string>

#ifdef parse
#undef parse
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

#ifdef PARSE_TEST_I
#undef PARSE_TEST_I
#endif

inline bool PARSE_TEST_I(const char* msg, CharParser p,
                         const char* input) {
  return parseTest_Char(msg, p, input);
}

inline bool PARSE_TEST_I(const char* msg, StringParser p,
                         const char* input) {
  return parseTest_String(msg, p, input);
}

inline bool PARSE_TEST_I(const char* msg, TokenParser p,
                         const char* input) {
  return parseTest_Token(msg, p, input);
}

#ifdef seq
#undef seq
template <typename... Parser>
inline StringParser seq(CharParser p, Parser... args) {
  CharParser ps[] = {p, args..., NULL};
  return seq_char(ps);
}
#endif
#ifdef SEQ_I
#undef SEQ_I
#endif

#ifdef cons
#undef cons
inline StringParser cons(CharParser p, StringParser ps) {
  return cons_char(p, ps);
}
#endif

#ifdef token
#undef token
inline TokenParser token(int type, char c) { return token_c(type, c); }
inline TokenParser token(int type, const char* s) {
  return token_s(type, s);
}
inline TokenParser token(int type, CharParser p) {
  return token_Char(type, p);
}
inline TokenParser token(int type, StringParser p) {
  return token_String(type, p);
}
#endif
