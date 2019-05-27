/* -*- coding: utf-8-unix -*- */

#include <catch.hpp>
#include <cparsec2.hpp>

SCENARIO("upper", "[cparsec2][parser][upper]") {
  cparsec2_init();
  GIVEN("an input: \"ABC\"") {
    Source src = Source_new("ABC");
    WHEN("apply upper") {
      THEN("results 'A'") {
        REQUIRE('A' == parse(upper, src));
      }
    }
  }
  GIVEN("an input: \"BC\"") {
    Source src = Source_new("BC");
    WHEN("apply upper") {
      THEN("results 'B'") {
        REQUIRE('B' == parse(upper, src));
      }
    }
  }
  GIVEN("an input: \"123\"") {
    Source src = Source_new("123");
    WHEN("apply upper") {
      THEN("cause exception(\"not satisfy\")") {
        REQUIRE_THROWS_WITH(parse(upper, src), "not satisfy");
      }
    }
  }
  GIVEN("an input: \"abc\"") {
    Source src = Source_new("abc");
    WHEN("apply upper") {
      THEN("cause exception(\"not satisfy\")") {
        REQUIRE_THROWS_WITH(parse(upper, src), "not satisfy");
      }
    }
  }
  GIVEN("an input: \"\"") {
    Source src = Source_new("");
    WHEN("apply upper") {
      THEN("cause exception(\"too short\")") {
        REQUIRE_THROWS_WITH(parse(upper, src), "too short");
      }
    }
  }
  cparsec2_end();
}
