/* -*- coding: utf-8-unix -*- */

#include <catch.hpp>
#include <cparsec2.hpp>

SCENARIO("digit", "[cparsec2][parser]") {
  cparsec2_init();
  GIVEN("an input: \"123\"") {
    Source src = Source_new("123");
    WHEN("apply digit") {
      THEN("results '1'") {
        REQUIRE('1' == parse(digit, src));
      }
    }
  }
  GIVEN("an input: \"23\"") {
    Source src = Source_new("23");
    WHEN("apply digit") {
      THEN("results '2'") {
        REQUIRE('2' == parse(digit, src));
      }
    }
  }
  GIVEN("an input: \"abc\"") {
    Source src = Source_new("abc");
    WHEN("apply digit") {
      THEN("cause exception(\"not satisfy\")") {
        REQUIRE_THROWS_WITH(parse(digit, src), "not satisfy");
      }
    }
  }
  GIVEN("an input: \"\"") {
    Source src = Source_new("");
    WHEN("apply digit") {
      THEN("cause exception(\"too short\")") {
        REQUIRE_THROWS_WITH(parse(digit, src), "too short");
      }
    }
  }
  cparsec2_end();
}
