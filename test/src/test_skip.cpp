/* -*- coding: utf-8-unix -*- */

#include <catch.hpp>
#include <cparsec2.hpp>

SCENARIO("skip", "[cparsec2][parser][skip]") {
  cparsec2_init();
  GIVEN("an input: \"abc\"") {
    Source src = Source_new("abc");
    WHEN("apply skip(char1('a'))") {
      THEN("success") {
        REQUIRE_NOTHROW(parse(skip(char1('a')), src));
        AND_WHEN("apply skip(string1(\"bc\"))") {
          THEN("success") {
            REQUIRE_NOTHROW(parse(skip(string1("bc")), src));
          }
        }
      }
    }
    WHEN("apply skip(char1('b'))") {
      THEN("cause exception(\"expects 'b' but was 'a'\")") {
        REQUIRE_THROWS_WITH(parse(skip(char1('b')), src),
                            "expects 'b' but was 'a'");
      }
    }
    WHEN("apply skip(string1(\"ab\"))") {
      THEN("success") {
        REQUIRE_NOTHROW(parse(skip(string1("ab")), src));
        AND_WHEN("apply skip(char1('c'))") {
          THEN("success") {
            REQUIRE_NOTHROW(parse(skip(char1('c')), src));
          }
        }
      }
    }
    WHEN("apply skip(string1(\"bc\"))") {
      THEN("cause exception(\"expects 'b' but was 'a'\")") {
        REQUIRE_THROWS_WITH(parse(skip(string1("bc")), src),
                            "expects 'b' but was 'a'");
      }
    }
  }
  cparsec2_end();
}