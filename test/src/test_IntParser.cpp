/* -*- coding: utf-8-unix -*- */

#include <catch.hpp>
#include <cparsec2.hpp>

extern "C" {
static int number_(void* arg, Source src, Ctx* ex) {
  UNUSED(arg);
  return atol(PARSE(String)(many1(digit), src, ex));
}
}

SCENARIO("PARSER(Int) number", "[cparsec2][parser][IntParser]") {
  cparsec2_init();
  PARSER(Int) number = PARSER_GEN(Int)(number_, NULL);
  GIVEN("an input: \"100\"") {
    Source src = Source_new("100");
    WHEN("apply number") {
      THEN("results 100") { REQUIRE(100 == parse(number, src)); }
    }
  }
  GIVEN("an input: \"100+200\"") {
    Source src = Source_new("100+200");
    WHEN("apply number -> x") {
      int x = parse(number, src);
      THEN("results x = 100") {
        REQUIRE(x == 100);
        AND_WHEN("apply char1('+')") {
          THEN("results '+'") {
            REQUIRE('+' == parse(char1('+'), src));
            AND_WHEN("apply number -> y") {
              int y = parse(number, src);
              THEN("results y = 200") {
                REQUIRE(y == 200);
                AND_THEN("x + y = 300") { REQUIRE(x + y == 300); }
              }
            }
          }
        }
      }
    }
  }
  cparsec2_end();
}
