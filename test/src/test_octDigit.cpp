/* -*- coding: utf-8-unix -*- */

#include <catch.hpp>
#include <cparsec2.hpp>

SCENARIO("octDigit", "[cparsec2][parser][octDigit]") {
  cparsec2_init();
  GIVEN("an input: \"123\"") {
    Source src = Source_new("123");
    WHEN("apply octDigit") {
      THEN("results '1'") {
        REQUIRE('1' == parse(octDigit, src));
      }
    }
  }
  GIVEN("an input: \"23\"") {
    Source src = Source_new("23");
    WHEN("apply octDigit") {
      THEN("results '2'") {
        REQUIRE('2' == parse(octDigit, src));
      }
    }
  }
  GIVEN("an input: \"abc\"") {
    Source src = Source_new("abc");
    WHEN("apply octDigit") {
      THEN("cause exception(\"not satisfy\")") {
        REQUIRE_THROWS_WITH(parse(octDigit, src), "not satisfy");
      }
    }
  }
  GIVEN("an input: \"\"") {
    Source src = Source_new("");
    WHEN("apply octDigit") {
      THEN("cause exception(\"too short\")") {
        REQUIRE_THROWS_WITH(parse(octDigit, src), "too short");
      }
    }
  }
  GIVEN("an input: \"0123456789abcdefg\"") {
    Source src = Source_new("0123456789abcdefg");
    WHEN("apply many1(octDigit)") {
      THEN("reaulst \"01234567\"") {
        REQUIRE(std::string("01234567") == parse(many1(octDigit), src));
      }
    }
  }
  GIVEN("an input: \"0123456789ABCDEFG\"") {
    Source src = Source_new("0123456789ABCDEFG");
    WHEN("apply many1(octDigit)") {
      THEN("reaulst \"01234567\"") {
        REQUIRE(std::string("01234567") == parse(many1(octDigit), src));
      }
    }
  }
  cparsec2_end();
}
