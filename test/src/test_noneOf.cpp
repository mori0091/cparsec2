/* -*- coding: utf-8-unix -*- */

#include <catch.hpp>
#include <cparsec2.hpp>

SCENARIO("noneOf", "[cparsec2][parser][noneOf]") {
  cparsec2_init();
  GIVEN("an input: \"a\"") {
    Source src = Source_new("a");
    WHEN("apply noneOf(\"abc\")") {
      THEN("cause exception(\"expects not one of \"abc\" but was 'a'\")") {
        REQUIRE_THROWS_WITH(parse(noneOf("abc"), src),
                            "expects not one of \"abc\" but was 'a'");
      }
    }
  }
  GIVEN("an input: \"b\"") {
    Source src = Source_new("b");
    WHEN("apply noneOf(\"abc\")") {
      THEN("cause exception(\"expects not one of \"abc\" but was 'b'\")") {
        REQUIRE_THROWS_WITH(parse(noneOf("abc"), src),
                            "expects not one of \"abc\" but was 'b'");
      }
    }
  }
  GIVEN("an input: \"c\"") {
    Source src = Source_new("c");
    WHEN("apply noneOf(\"abc\")") {
      THEN("cause exception(\"expects not one of \"abc\" but was 'c'\")") {
        REQUIRE_THROWS_WITH(parse(noneOf("abc"), src),
                            "expects not one of \"abc\" but was 'c'");
      }
    }
  }
  GIVEN("an input: \"d\"") {
    Source src = Source_new("d");
    WHEN("apply noneOf(\"abc\")") {
      THEN("results 'd'") {
        REQUIRE('d' == parse(noneOf("abc"), src));
      }
    }
  }
  GIVEN("an input: \"\"") {
    Source src = Source_new("");
    WHEN("apply noneOf(\"abc\")") {
      THEN("cause exception(\"too short\")") {
        REQUIRE_THROWS_WITH(parse(noneOf("abc"), src), "too short");
      }
    }
  }
  cparsec2_end();
}
