/* -*- coding: utf-8-unix -*- */

#include <catch.hpp>
#include <cparsec2.hpp>

SCENARIO("tryp(p)", "[cparsec2][parser][tryp]") {
  cparsec2_init();

  GIVEN("an input \"123\"") {
    Source src = Source_new("123");
    WHEN("applied 'tryp(many(digit))'") {
      THEN("resuls \"123\"") {
        REQUIRE("123" == parse(tryp(many(digit)), src));
      }
    }
    WHEN("applied 'tryp(char1('a'))'") {
      THEN("causes an exception \"expects 'a' but was '1'\"") {
        const char* previous_state = src->p;
        REQUIRE_THROWS_WITH(parse(tryp(char1('a')), src),
                            "expects 'a' but was '1'");
        AND_THEN("input-state is not changed") {
          REQUIRE(src->p == previous_state);
        }
      }
    }
  }

  GIVEN("a PARSER(String) p = either(tryp(\"ab\"), string1(\"bc\"))") {
    PARSER(String) p = either(tryp("ab"), string1("bc"));
    WHEN("an input was \"ab\"") {
      Source src = Source_new("ab");
      THEN("applied 'p' results \"ab\"") {
        REQUIRE("ab" == parse(p, src));
      }
    }
    WHEN("an input was \"bc\"") {
      Source src = Source_new("bc");
      THEN("applied 'p' results \"bc\"") {
        REQUIRE("bc" == parse(p, src));
      }
    }
    WHEN("an input was \"ac\"") {
      Source src = Source_new("ac");
      THEN("applied 'p' causes an exception "
           "\"expects 'b' but was 'a'\"") {
        REQUIRE_THROWS_WITH(parse(p, src), "expects 'b' but was 'a'");
      }
    }
    WHEN("an input was \"bd\"") {
      Source src = Source_new("bd");
      THEN("applied 'p' causes an exception "
           "\"expects 'c' but was 'd'\"") {
        REQUIRE_THROWS_WITH(parse(p, src), "expects 'c' but was 'd'");
      }
    }
    WHEN("an input was \"b\"") {
      Source src = Source_new("b");
      THEN("applied 'p' causes an exception "
           "\"too short\"") {
        REQUIRE_THROWS_WITH(parse(p, src), "too short");
      }
    }
  }

  cparsec2_end();
}
