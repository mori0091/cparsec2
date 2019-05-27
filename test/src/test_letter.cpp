/* -*- coding: utf-8-unix -*- */

#include <catch.hpp>
#include <cparsec2.hpp>

SCENARIO("letter", "[cparsec2][parser][letter]") {
  cparsec2_init();
  GIVEN("an input: \"ABC\"") {
    Source src = Source_new("ABC");
    WHEN("apply letter") {
      THEN("results 'A'") {
        REQUIRE('A' == parse(letter, src));
      }
    }
  }
  GIVEN("an input: \"BC\"") {
    Source src = Source_new("BC");
    WHEN("apply letter") {
      THEN("results 'B'") {
        REQUIRE('B' == parse(letter, src));
      }
    }
  }
  GIVEN("an input: \"123\"") {
    Source src = Source_new("123");
    WHEN("apply letter") {
      THEN("cause exception(\"not satisfy\")") {
        REQUIRE_THROWS_WITH(parse(letter, src), "not satisfy");
      }
    }
  }
  GIVEN("an input: \"abc\"") {
    Source src = Source_new("abc");
    WHEN("apply letter") {
      THEN("results 'a'") {
        REQUIRE('a' == parse(letter, src));
      }
    }
  }
  GIVEN("an input: \"_+*\"") {
    Source src = Source_new("_+*");
    WHEN("apply letter") {
      THEN("results '_'") {
        REQUIRE('_' == parse(letter, src));
      }
    }
  }
  GIVEN("an input: \"+*\"") {
    Source src = Source_new("+*");
    WHEN("apply letter") {
      THEN("cause exception(\"not satisfy\")") {
        REQUIRE_THROWS_WITH(parse(letter, src), "not satisfy");
      }
    }
  }
  GIVEN("an input: \"\"") {
    Source src = Source_new("");
    WHEN("apply letter") {
      THEN("cause exception(\"too short\")") {
        REQUIRE_THROWS_WITH(parse(letter, src), "too short");
      }
    }
  }
  cparsec2_end();
}
