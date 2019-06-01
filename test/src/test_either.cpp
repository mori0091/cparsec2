/* -*- coding: utf-8-unix -*- */

#include <catch.hpp>
#include <cparsec2.hpp>

SCENARIO("either(string1(\"ab\"), string1(\"bc\"))", "[cparsec2][parser][either]") {
  cparsec2_init();
  GIVEN("PARSER(String) p = either(string1(\"ab\"), string1(\"bc\"))") {
    PARSER(String)p = either(string1("ab"), string1("bc"));
    WHEN("applied to an input: \"ab\"") {
      Source src = Source_new("ab");
      THEN("results \"ab\"") {
        REQUIRE("ab" == parse(p, src));
      }
    }
    WHEN("applied to an input: \"bc\"") {
      Source src = Source_new("bc");
      THEN("results \"bc\"") {
        REQUIRE("bc" == parse(p, src));
      }
    }
    WHEN("applied to an input: \"ac\"") {
      Source src = Source_new("ac");
      THEN("causes an error \"expects 'b' but was 'c'\"") {
        REQUIRE_THROWS_WITH(parse(p, src), "expects 'b' but was 'c'");
      }
    }
    WHEN("applied to an input: \"bd\"") {
      Source src = Source_new("bd");
      THEN("causes an error \"expects 'c' but was 'd'\"") {
        REQUIRE_THROWS_WITH(parse(p, src), "expects 'c' but was 'd'");
      }
    }
    WHEN("applied to an input: \"b\"") {
      Source src = Source_new("b");
      THEN("causes an error \"too short\"") {
        REQUIRE_THROWS_WITH(parse(p, src), "too short");
      }
    }
  }
  cparsec2_end();
}
