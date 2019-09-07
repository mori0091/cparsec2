/* -*- coding:utf-8-unix -*- */

#include <cparsec2.h>

// ---- built-in parsers ----

CharParser anyChar;
CharParser digit;
CharParser hexDigit;
CharParser octDigit;
CharParser lower;
CharParser upper;
CharParser alpha;
CharParser alnum;
CharParser letter;
CharParser space;
StringParser spaces;
CharParser newline;
CharParser crlf;
CharParser endOfLine;
CharParser endOfFile;
CharParser tab;
IntParser number;
StringParser anyUtf8;

static char endOfFile_fn(void* arg, Source src, Ctx* ex) {
  UNUSED(arg);
  Ctx ctx;
  TRY(&ctx) {
    char c = peek(src, &ctx);
    cthrow(ex, mem_printf("expects <eof> but was '%c'", c));
  }
  else {
    mem_free((void*)ctx.msg);
  }
  return 0;
}

static int number_fn(void* arg, Source src, Ctx* ex) {
  PARSER(String) p = (PARSER(String))arg;
  return atoi(parse(p, src, ex));
}

static void cparsec2_init__stage1(void) {
  anyChar = satisfy(is_anyChar);
  digit = expects("a decimal digit", satisfy(is_digit));
  hexDigit = expects("a hexa-decimal digit", satisfy(is_hexDigit));
  octDigit = expects("an octal digit", satisfy(is_octDigit));
  lower = expects("a lower-case character", satisfy(is_lower));
  upper = expects("a upper-case character", satisfy(is_upper));
  alpha = expects("an alphabet", satisfy(is_alpha));
  alnum = expects("an alphabet or a decimal digit", satisfy(is_alnum));
  letter = expects("a letter", satisfy(is_letter));
  space = expects("one of a white-space", satisfy(is_space));
}

static void cparsec2_init__stage2(void) {
  spaces = many(space);
  newline = expects("<LF>", char1('\n'));
  crlf = expects("<CR><LF>", skip1st(char1('\r'), newline));
  endOfLine = expects("<endOfLine>", either(newline, crlf));
  endOfFile = expects("<endOfFile>", PARSER_GEN(Char)(endOfFile_fn, NULL));
  tab = expects("a TAB", char1('\t'));
  number = PARSER_GEN(Int)(number_fn, token(many1(digit)));
}

static void cparsec2_init__stage3(void) {
  PARSER(Char) t = range(0x80, 0xBF);
  PARSER(String) u1 = seq(range(0x00, 0x7F));
  PARSER(String) u2 = seq(range(0xC2, 0xDF), t);
  PARSER(String) u3a = seq(char1((char)0xE0), range(0xA0, 0xBF), t);
  PARSER(String) u3b = seq(range(0xE1, 0xEC), t, t);
  PARSER(String) u3c = seq(char1((char)0xED), range(0x80, 0x9F));
  PARSER(String) u3d = seq(range(0xEE, 0xEF), t, t);
  PARSER(String) u4a = seq(char1((char)0xF0), range(0x90, 0xBF), t, t);
  PARSER(String) u4b = seq(range(0xF1, 0xF3), t, t, t);
  PARSER(String) u4c = seq(char1((char)0xF4), range(0x80, 0xBF), t, t);
  anyUtf8 = TREE_FOLDL(EITHER(String), u1, u2, u3a, u3b, u3c, u3d, u4a,
                       u4b, u4c);
}

void cparsec2_create_builtin_parsers(void) {
  cparsec2_init__stage1();
  cparsec2_init__stage2();
  cparsec2_init__stage3();
}
