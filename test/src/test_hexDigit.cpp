/* -*- coding: utf-8-unix -*- */

#include <catch.hpp>
#include <cparsec2.hpp>

SCENARIO("hexDigit", "[cparsec2][parser][hexDigit]") {
  cparsec2_init();
  GIVEN("an input: \"123\"") {
    Source src = Source_new("123");
    WHEN("apply hexDigit") {
      THEN("results '1'") {
        REQUIRE('1' == parse(hexDigit, src));
      }
    }
  }
  GIVEN("an input: \"23\"") {
    Source src = Source_new("23");
    WHEN("apply hexDigit") {
      THEN("results '2'") {
        REQUIRE('2' == parse(hexDigit, src));
      }
    }
  }
  GIVEN("an input: \"abc\"") {
    Source src = Source_new("abc");
    WHEN("apply hexDigit") {
      THEN("results 'a'") {
        REQUIRE('a' == parse(hexDigit, src));
      }
    }
  }
  GIVEN("an input: \"\"") {
    Source src = Source_new("");
    WHEN("apply hexDigit") {
      THEN("cause exception(\"too short\")") {
        REQUIRE_THROWS_WITH(parse(hexDigit, src), "too short");
      }
    }
  }
  GIVEN("an input: \"0123456789abcdefg\"") {
    Source src = Source_new("0123456789abcdefg");
    WHEN("apply many1(hexDigit)") {
      THEN("reaulst \"0123456789abcdef\"") {
        REQUIRE("0123456789abcdef" == parse(many1(hexDigit), src));
      }
    }
  }
  GIVEN("an input: \"0123456789ABCDEFG\"") {
    Source src = Source_new("0123456789ABCDEFG");
    WHEN("apply many1(hexDigit)") {
      THEN("reaulst \"0123456789ABCDEF\"") {
        REQUIRE("0123456789ABCDEF" == parse(many1(hexDigit), src));
      }
    }
  }
  cparsec2_end();
}
