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
  GIVEN("an input \"abc\"") {
    Source src = Source_new("abc");
    WHEN("apply many(number)") {
      List(Int) xs = parse(many(number), src);
      THEN("results []") {
        REQUIRE(list_begin(xs) == list_end(xs));
      }
    }
    WHEN("apply many(token(many1(digit)))") {
      List(String) xs = parse(many(token(many1(digit))), src);
      THEN("results []") {
        REQUIRE(list_begin(xs) == list_end(xs));
      }
    }
  }
  GIVEN("an input \",123,456,789\"") {
    Source src = Source_new(",123,456,789");
    WHEN("apply many(skip1st(char1(','), number))") {
      List(Int) xs = parse(many(skip1st(char1(','), number)), src);
      THEN("results [123, 456, 789]") {
        int* itr = list_begin(xs);
        REQUIRE(123 == itr[0]);
        REQUIRE(456 == itr[1]);
        REQUIRE(789 == itr[2]);
      }
    }
    WHEN("apply many(skip1st(char1(','), many1(digit)))") {
      List(String) xs =
          parse(many(skip1st(char1(','), many1(digit))), src);
      THEN("results [\"123\", \"456\", \"789\"]") {
        const char** itr = list_begin(xs);
        REQUIRE("123" == std::string(itr[0]));
        REQUIRE("456" == std::string(itr[1]));
        REQUIRE("789" == std::string(itr[2]));
      }
    }
  }
  cparsec2_end();
}
