/* -*- coding: utf-8-unix -*- */

#include <catch.hpp>
#include <cparsec2.hpp>

SCENARIO("lower", "[cparsec2][parser][lower]") {
  cparsec2_init();
  GIVEN("an input: \"abc\"") {
    Source src = Source_new("abc");
    WHEN("apply lower") {
      THEN("results 'a'") {
        REQUIRE('a' == parse(lower, src));
      }
    }
  }
  GIVEN("an input: \"bc\"") {
    Source src = Source_new("bc");
    WHEN("apply lower") {
      THEN("results 'b'") {
        REQUIRE('b' == parse(lower, src));
      }
    }
  }
  GIVEN("an input: \"123\"") {
    Source src = Source_new("123");
    WHEN("apply lower") {
      THEN("cause exception(\"not satisfy\")") {
        REQUIRE_THROWS_WITH(parse(lower, src), "not satisfy");
      }
    }
  }
  GIVEN("an input: \"ABC\"") {
    Source src = Source_new("ABC");
    WHEN("apply lower") {
      THEN("cause exception(\"not satisfy\")") {
        REQUIRE_THROWS_WITH(parse(lower, src), "not satisfy");
      }
    }
  }
  GIVEN("an input: \"\"") {
    Source src = Source_new("");
    WHEN("apply lower") {
      THEN("cause exception(\"too short\")") {
        REQUIRE_THROWS_WITH(parse(lower, src), "too short");
      }
    }
  }
  cparsec2_end();
}
