/* -*- coding: utf-8-unix -*- */

#include <catch.hpp>
#include <cparsec2.hpp>

SCENARIO("skip2nd", "[cparsec2][parser][skip2nd]") {
  cparsec2_init();
  GIVEN("an input: \"abc\"") {
    Source src = Source_new("abc");
    WHEN("apply skip2nd(char1('a'), string1(\"bc\"))") {
      THEN("results 'a'") {
        REQUIRE('a' == parse(skip2nd(char1('a'), string1("bc")), src));
      }
    }
    WHEN("apply skip2nd(char1('X'), string1(\"bc\"))") {
      THEN("cause exception(\"expects 'X' but was 'a'\")") {
        REQUIRE_THROWS_WITH(
            parse(skip2nd(char1('X'), string1("bc")), src),
            "expects 'X' but was 'a'");
      }
    }
    WHEN("apply skip2nd(char1('a'), string1(\"XY\"))") {
      THEN("cause exception(\"expects 'X' but was 'b'\")") {
        REQUIRE_THROWS_WITH(
            parse(skip2nd(char1('a'), string1("XY")), src),
            "expects 'X' but was 'b'");
      }
    }
    WHEN("apply skip2nd(string1(\"ab\"), char1('c'))") {
      THEN("results \"ab\"") {
        REQUIRE(std::string("ab") ==
                parse(skip2nd(string1("ab"), char1('c')), src));
      }
    }
    WHEN("apply skip2nd(string1(\"aX\"), char1('c'))") {
      THEN("cause exception(\"expects 'X' but was 'b'\")") {
        REQUIRE_THROWS_WITH(
            parse(skip2nd(string1("aX"), char1('c')), src),
            "expects 'X' but was 'b'");
      }
    }
    WHEN("apply skip2nd(string1(\"ab\"), char1('X'))") {
      THEN("cause exception(\"expects 'X' but was 'c'\")") {
        REQUIRE_THROWS_WITH(
            parse(skip2nd(string1("ab"), char1('X')), src),
            "expects 'X' but was 'c'");
      }
    }
  }
  cparsec2_end();
}