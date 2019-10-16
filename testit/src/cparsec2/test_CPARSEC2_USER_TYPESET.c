/* -*- coding: utf-8-unix -*- */
#define TestSuite USER_TYPESET
#include "testit.h"

#define CPARSEC2_USER_TYPESET int8_t, uint8_t
#include <cparsec2.h>

CPARSEC2_DEFINE_USER_TYPESET();

#include <inttypes.h>

void SHOW(int8_t)(int8_t x) {
  printf("%" PRId8, x);
}

void SHOW(uint8_t)(uint8_t x) {
  printf("%" PRIu8, x);
}

test("Buff(int8_t) and List(int8_t)") {
  cparsec2_init();

  Buff(int8_t) b = {0};
  buff_push(&b, 0);
  buff_push(&b, 1);
  buff_push(&b, 2);
  buff_push(&b, 127);
  buff_push(&b, -128);
  List(int8_t) xs = buff_finish(&b);
  c_assert(eq(0, list_begin(xs)[0]));
  c_assert(eq(1, list_begin(xs)[1]));
  c_assert(eq(2, list_begin(xs)[2]));
  c_assert(eq(127, list_begin(xs)[3]));
  c_assert(eq(-128, list_begin(xs)[4]));

  cparsec2_end();
}

test("Buff(uint8_t) and List(uint8_t)") {
  cparsec2_init();

  Buff(uint8_t) b = {0};
  buff_push(&b, 0);
  buff_push(&b, 1);
  buff_push(&b, 2);
  buff_push(&b, 255);
  List(uint8_t) xs = buff_finish(&b);
  c_assert(eq(0, list_begin(xs)[0]));
  c_assert(eq(1, list_begin(xs)[1]));
  c_assert(eq(2, list_begin(xs)[2]));
  c_assert(eq(255, list_begin(xs)[3]));

  cparsec2_end();
}

#include <assert.h>

static uint8_t hex2u(char c) {
  if ('0' <= c && c <= '9') {
    return (uint8_t)(c - '0');
  }
  if ('a' <= c && c <= 'f') {
    return (uint8_t)(c - 'a' + 10);
  }
  if ('A' <= c && c <= 'F') {
    return (uint8_t)(c - 'A' + 10);
  }
  assert(0 && "not a hexadecimal digit");
  return 0;
}

static uint8_t hex_fn(void* arg, Source src, Ctx* ex) {
  UNUSED(arg);
  char hi = parse(hexDigit, src, ex);
  char lo = parse(hexDigit, src, ex);
  return ((hex2u(hi) << 4) | hex2u(lo));
}

test("PARSER(uint8_t) hex") {
  cparsec2_init();

  PARSER(uint8_t) hex = PARSER_GEN(uint8_t)(hex_fn, NULL);

  ParseResult(List(uint8_t)) r = runParser(many1(hex), "12aB");
  c_assert(r.succeeded);
  c_assert(eq(4, r.consumed));
  c_assert(eq(0x12, list_begin(r.ok)[0]));
  c_assert(eq(0xab, list_begin(r.ok)[1]));

  cparsec2_end();
}
