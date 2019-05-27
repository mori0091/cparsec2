/* -*- coding: utf-8-unix -*- */

#include <catch.hpp>
#include <cparsec2.hpp>

SCENARIO("cons(letter, many(digit))", "[cparsec2][parser][cons]") {
  cparsec2_init();
  GIVEN("an input: \"a1234\"") {
    Source src = Source_new("a1234");
    WHEN("apply cons(letter, many(digit))") {
      THEN("results \"a1234\"") {
        REQUIRE("a1234" == parse(cons(letter, many(digit)), src));
      }
    }
  }
  GIVEN("an input: \"abc123\"") {
    Source src = Source_new("abc123");
    WHEN("apply cons(letter, many(digit))") {
      THEN("results \"a\"") {
        REQUIRE("a" == parse(cons(letter, many(digit)), src));
      }
    }
  }
  GIVEN("an input: \"a123bc\"") {
    Source src = Source_new("a123bc");
    WHEN("apply cons(letter, many(digit))") {
      THEN("results \"a123\"") {
        REQUIRE("a123" == parse(cons(letter, many(digit)), src));
      }
    }
  }
  GIVEN("an input: \"123\"") {
    Source src = Source_new("123");
    WHEN("apply cons(letter, many(digit))") {
      THEN("cause exception(\"not satisfy\")") {
        REQUIRE_THROWS_WITH(parse(cons(letter, many(digit)), src),
                            "not satisfy");
      }
    }
  }
  cparsec2_end();
}
