/* -*- coding: utf-8-unix -*- */

#include <catch.hpp>
#include <cparsec2.hpp>

SCENARIO("alnum", "[cparsec2][parser][alnum]") {
  cparsec2_init();
  GIVEN("an input: \"ABC\"") {
    Source src = Source_new("ABC");
    WHEN("apply alnum") {
      THEN("results 'A'") {
        REQUIRE('A' == parse(alnum, src));
      }
    }
  }
  GIVEN("an input: \"BC\"") {
    Source src = Source_new("BC");
    WHEN("apply alnum") {
      THEN("results 'B'") {
        REQUIRE('B' == parse(alnum, src));
      }
    }
  }
  GIVEN("an input: \"123\"") {
    Source src = Source_new("123");
    WHEN("apply alnum") {
      THEN("results '1'") {
        REQUIRE('1' == parse(alnum, src));
      }
    }
  }
  GIVEN("an input: \"abc\"") {
    Source src = Source_new("abc");
    WHEN("apply alnum") {
      THEN("results 'a'") {
        REQUIRE('a' == parse(alnum, src));
      }
    }
  }
  GIVEN("an input: \"\"") {
    Source src = Source_new("");
    WHEN("apply alnum") {
      THEN("cause exception(\"too short\")") {
        REQUIRE_THROWS_WITH(parse(alnum, src), "too short");
      }
    }
  }
  cparsec2_end();
}
