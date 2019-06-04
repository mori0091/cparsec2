/* -*- coding: utf-8-unix -*- */

#include <catch.hpp>
#include <cparsec2.hpp>

SCENARIO("many(p)", "[cparsec2][parser][many]") {
  cparsec2_init();
  GIVEN("an input \"1234567890abc\"") {
    Source src = Source_new("1234567890abc");
    WHEN("apply many(digit)") {
      THEN("results \"1234567890\"") {
        REQUIRE("1234567890" == parse(many(digit), src));
      }
    }
  }
  GIVEN("an input \"aaabbb\"") {
    Source src = Source_new("aaabbb");
    WHEN("apply many(digit)") {
      THEN("results \"\"") {
        REQUIRE("" == parse(many(digit), src));
      }
    }
    WHEN("apply many(char1('a'))") {
      AND_WHEN("apply many(char1('c'))") {
        THEN("results \"aaa\"") {
          REQUIRE("aaa" == parse(many(char1('a')), src));
          AND_THEN("results \"\"") {
            REQUIRE("" == parse(many(char1('c')), src));
          }
        }
      }
    }
    WHEN("apply many(char1('a'))") {
      AND_WHEN("apply many(char1('b'))") {
        AND_WHEN("apply a parser (ex. alpha)") {
          THEN("results \"aaa\"") {
            REQUIRE("aaa" == parse(many(char1('a')), src));
            AND_THEN("results \"bbb\"") {
              REQUIRE("bbb" == parse(many(char1('b')), src));
              AND_THEN("cause exception(\"too short\")") {
                REQUIRE_THROWS_WITH(parse(alpha, src), "too short");
              }
            }
          }
        }
      }
    }
  }
  cparsec2_end();
}
