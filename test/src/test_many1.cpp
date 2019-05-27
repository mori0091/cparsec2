/* -*- coding: utf-8-unix -*- */

#include <cparsec2.hpp>
#include <catch.hpp>

SCENARIO("many1(p)", "[cparsec2][parser]") {
  cparsec2_init();
  GIVEN("an input \"aaabbb\"") {
    Source src = Source_new("aaabbb");
    WHEN("apply many1(digit)") {
      THEN("cause exception(\"not satisfy\")") {
        REQUIRE_THROWS_WITH(parse(many1(digit), src), "not satisfy");
      }
    }
    WHEN("apply many1(char1('a'))") {
      AND_WHEN("apply many1(char1('b'))") {
        AND_WHEN("apply a parser (ex. alpha)") {
          THEN("results \"aaa\"") {
            REQUIRE("aaa" == parse(many1(char1('a')), src));
            AND_THEN("results \"bbb\"") {
              REQUIRE("bbb" == parse(many1(char1('b')), src));
              AND_THEN("cause exception(\"too short\")") {
                REQUIRE_THROWS_WITH(parse(alpha, src), "too short");
              }
            }
          }
        }
      }
    }
  }
  cparsec2_end();
}
