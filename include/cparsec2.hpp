/* -*- coding:utf-8-unix -*- */
#pragma once

#include <cparsec2.h>
#include <string>

#ifdef parse
#undef parse
#endif

inline char parse(PARSER(Char) p, Source src) {
  Ctx ctx;
  TRY(&ctx) { return PARSE(Char)(p, src, &ctx); }
  else {
    std::string ex(ctx.msg);
    throw ex;
  }
}

inline std::string parse(PARSER(String) p, Source src) {
  Ctx ctx;
  TRY(&ctx) { return PARSE(String)(p, src, &ctx); }
  else {
    std::string ex(ctx.msg);
    throw ex;
  }
}

inline Token parse(PARSER(Token) p, Source src) {
  Ctx ctx;
  TRY(&ctx) { return PARSE(Token)(p, src, &ctx); }
  else {
    std::string ex(ctx.msg);
    throw ex;
  }
}

#ifdef PARSE_TEST_I
#undef PARSE_TEST_I
#endif

inline bool PARSE_TEST_I(const char* msg, PARSER(Char) p,
                         const char* input) {
  return PARSETEST(Char)(msg, p, input);
}

inline bool PARSE_TEST_I(const char* msg, PARSER(String) p,
                         const char* input) {
  return PARSETEST(String)(msg, p, input);
}

inline bool PARSE_TEST_I(const char* msg, PARSER(Token) p,
                         const char* input) {
  return PARSETEST(Token)(msg, p, input);
}

#ifdef seq
#undef seq
template <typename... Parser>
inline PARSER(String) seq(PARSER(Char) p, Parser... args) {
  PARSER(Char) ps[] = {p, args..., NULL};
  return seq_char(ps);
}
#endif
#ifdef SEQ_I
#undef SEQ_I
#endif

#ifdef cons
#undef cons
inline PARSER(String) cons(PARSER(Char) p, PARSER(String) ps) {
  return cons_char(p, ps);
}
#endif

#ifdef token
#undef token
inline PARSER(Token) token(int type, char c) { return token_c(type, c); }
inline PARSER(Token) token(int type, const char* s) {
  return token_s(type, s);
}
inline PARSER(Token) token(int type, PARSER(Char) p) {
  return token_Char(type, p);
}
inline PARSER(Token) token(int type, PARSER(String) p) {
  return token_String(type, p);
}
#endif
