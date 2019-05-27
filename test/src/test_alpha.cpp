/* -*- coding: utf-8-unix -*- */

#include <catch.hpp>
#include <cparsec2.hpp>

SCENARIO("alpha", "[cparsec2][parser][alpha]") {
  cparsec2_init();
  GIVEN("an input: \"ABC\"") {
    Source src = Source_new("ABC");
    WHEN("apply alpha") {
      THEN("results 'A'") {
        REQUIRE('A' == parse(alpha, src));
      }
    }
  }
  GIVEN("an input: \"BC\"") {
    Source src = Source_new("BC");
    WHEN("apply alpha") {
      THEN("results 'B'") {
        REQUIRE('B' == parse(alpha, src));
      }
    }
  }
  GIVEN("an input: \"123\"") {
    Source src = Source_new("123");
    WHEN("apply alpha") {
      THEN("cause exception(\"not satisfy\")") {
        REQUIRE_THROWS_WITH(parse(alpha, src), "not satisfy");
      }
    }
  }
  GIVEN("an input: \"abc\"") {
    Source src = Source_new("abc");
    WHEN("apply alpha") {
      THEN("results 'a'") {
        REQUIRE('a' == parse(alpha, src));
      }
    }
  }
  GIVEN("an input: \"\"") {
    Source src = Source_new("");
    WHEN("apply alpha") {
      THEN("cause exception(\"too short\")") {
        REQUIRE_THROWS_WITH(parse(alpha, src), "too short");
      }
    }
  }
  cparsec2_end();
}
