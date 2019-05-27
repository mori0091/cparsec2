/* -*- coding: utf-8-unix -*- */

#include <catch.hpp>
#include <cparsec2.hpp>

SCENARIO("char1(c)", "[cparsec2][parser][char1]") {
  cparsec2_init();
  GIVEN("an input: \"abc\" ") {
    Source src = Source_new("abc");
    WHEN("apply char1('a')") {
      THEN("results 'a'") { REQUIRE('a' == parse(char1('a'), src)); }
    }
    WHEN("apply char1('b')") {
      THEN("cause exception(\"expects 'b' but was 'a'\")") {
        REQUIRE_THROWS_WITH(parse(char1('b'), src),
                            "expects 'b' but was 'a'");
      }
    }
  }
  GIVEN("an input: \"\"") {
    Source src = Source_new("");
    WHEN("apply char1('a')") {
      THEN("cause exception(\"too short\")") {
        REQUIRE_THROWS_WITH(parse(char1('a'), src), "too short");
      }
    }
  }
  cparsec2_end();
}
