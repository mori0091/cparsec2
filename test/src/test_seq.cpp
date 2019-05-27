/* -*- coding: utf-8-unix -*- */

#include <catch.hpp>
#include <cparsec2.hpp>

SCENARIO("seq(letter, digit, digit)", "[cparsec2][parser][seq]") {
  cparsec2_init();
  GIVEN("an input: \"a12\"") {
    Source src = Source_new("a12");
    WHEN("apply seq(letter, digit, digit)") {
      THEN("results \"a12\"") {
        REQUIRE("a12" == parse(seq(letter, digit, digit), src));
      }
    }
  }
  GIVEN("an input: \"_123\"") {
    Source src = Source_new("_123");
    WHEN("apply seq(letter, digit, digit)") {
      THEN("results \"_12\"") {
        REQUIRE("_12" == parse(seq(letter, digit, digit), src));
      }
    }
  }
  GIVEN("an input: \"123\"") {
    Source src = Source_new("123");
    WHEN("apply seq(letter, digit, digit)") {
      THEN("cause exception(\"not satisfy\")") {
        REQUIRE_THROWS_WITH(parse(seq(letter, digit, digit), src),
                            "not satisfy");
      }
    }
  }
  cparsec2_end();
}
