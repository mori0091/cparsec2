/* -*- coding: utf-8-unix -*- */

#include <catch.hpp>
#include <cparsec2.hpp>

SCENARIO("oneOf", "[cparsec2][parser][oneOf]") {
  cparsec2_init();
  GIVEN("an input: \"a\"") {
    Source src = Source_new("a");
    WHEN("apply oneOf(\"abc\")") {
      THEN("results 'a'") {
        REQUIRE('a' == parse(oneOf("abc"), src));
      }
    }
  }
  GIVEN("an input: \"b\"") {
    Source src = Source_new("b");
    WHEN("apply oneOf(\"abc\")") {
      THEN("results 'b'") {
        REQUIRE('b' == parse(oneOf("abc"), src));
      }
    }
  }
  GIVEN("an input: \"c\"") {
    Source src = Source_new("c");
    WHEN("apply oneOf(\"abc\")") {
      THEN("results 'c'") {
        REQUIRE('c' == parse(oneOf("abc"), src));
      }
    }
  }
  GIVEN("an input: \"d\"") {
    Source src = Source_new("d");
    WHEN("apply oneOf(\"abc\")") {
      THEN("cause exception(\"expects one of \"abc\" but was 'd'\")") {
        REQUIRE_THROWS_WITH(parse(oneOf("abc"), src),
                            "expects one of \"abc\" but was 'd'");
      }
    }
  }
  GIVEN("an input: \"\"") {
    Source src = Source_new("");
    WHEN("apply oneOf(\"abc\")") {
      THEN("cause exception(\"too short\")") {
        REQUIRE_THROWS_WITH(parse(oneOf("abc"), src), "too short");
      }
    }
  }
  cparsec2_end();
}
