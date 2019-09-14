/* -*- coding: utf-8-unix -*- */

#include <catch.hpp>
#include <cparsec2.hpp>

SCENARIO("skip1st", "[cparsec2][parser][skip1st]") {
  cparsec2_init();
  GIVEN("an input: \"abc\"") {
    Source src = Source_new("abc");
    WHEN("apply skip1st(char1('a'), string1(\"bc\"))") {
      THEN("results \"bc\"") {
        REQUIRE(std::string("bc") ==
                parse(skip1st(char1('a'), string1("bc")), src));
      }
    }
    WHEN("apply skip1st(char1('X'), string1(\"bc\"))") {
      THEN("cause an error") {
        REQUIRE_THROWS(parse(skip1st(char1('X'), string1("bc")), src));
      }
    }
    WHEN("apply skip1st(char1('a'), string1(\"XY\"))") {
      THEN("cause an error") {
        REQUIRE_THROWS(parse(skip1st(char1('a'), string1("XY")), src));
      }
    }
    WHEN("apply skip1st(string1(\"ab\"), char1('c'))") {
      THEN("results 'c'") {
        REQUIRE('c' == parse(skip1st(string1("ab"), char1('c')), src));
      }
    }
    WHEN("apply skip1st(string1(\"aX\"), char1('c'))") {
      THEN("cause an error") {
        REQUIRE_THROWS(parse(skip1st(string1("aX"), char1('c')), src));
      }
    }
    WHEN("apply skip1st(string1(\"ab\"), char1('X'))") {
      THEN("cause an error") {
        REQUIRE_THROWS(parse(skip1st(string1("ab"), char1('X')), src));
      }
    }
  }
  cparsec2_end();
}
