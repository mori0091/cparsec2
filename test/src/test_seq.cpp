/* -*- coding: utf-8-unix -*- */

#include <catch.hpp>
#include <cparsec2.hpp>

SCENARIO("seq(letter, digit, digit)", "[cparsec2][parser][seq]") {
  cparsec2_init();
  GIVEN("an input: \"a12\"") {
    Source src = Source_new("a12");
    WHEN("apply seq(letter, digit, digit)") {
      THEN("results \"a12\"") {
        REQUIRE("a12" == parse(seq(letter, digit, digit), src));
      }
    }
  }
  GIVEN("an input: \"_123\"") {
    Source src = Source_new("_123");
    WHEN("apply seq(letter, digit, digit)") {
      THEN("results \"_12\"") {
        REQUIRE("_12" == parse(seq(letter, digit, digit), src));
      }
    }
  }
  GIVEN("an input: \"123\"") {
    Source src = Source_new("123");
    WHEN("apply seq(letter, digit, digit)") {
      THEN("cause exception(\"not satisfy\")") {
        REQUIRE_THROWS_WITH(parse(seq(letter, digit, digit), src),
                            "not satisfy");
      }
    }
  }
  cparsec2_end();
}

SCENARIO("seq", "[cparsec2][parser][seq]") {
  cparsec2_init();
  GIVEN("an input \"123 456 789\"") {
    Source src = Source_new("123 456 789");
    WHEN("apply seq(number, number, number)") {
      List(Int) xs = parse(seq(number, number, number), src);
      THEN("reaults [123, 456, 789]") {
        int* itr = list_begin(xs);
        REQUIRE(123 == itr[0]);
        REQUIRE(456 == itr[1]);
        REQUIRE(789 == itr[2]);
      }
    }
    WHEN("apply seq(x, x, x), where x = token(many1(digit))") {
      PARSER(String) x = token(many1(digit));
      List(String) xs = parse(seq(x, x, x), src);
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
