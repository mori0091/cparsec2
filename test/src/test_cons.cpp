/* -*- coding: utf-8-unix -*- */

#include <catch.hpp>
#include <cparsec2.hpp>

SCENARIO("cons(letter, many(digit))", "[cparsec2][parser][cons]") {
  cparsec2_init();
  GIVEN("an input: \"a1234\"") {
    Source src = Source_new("a1234");
    WHEN("apply cons(letter, many(digit))") {
      THEN("results \"a1234\"") {
        REQUIRE(std::string("a1234") ==
                parse(cons(letter, many(digit)), src));
      }
    }
  }
  GIVEN("an input: \"abc123\"") {
    Source src = Source_new("abc123");
    WHEN("apply cons(letter, many(digit))") {
      THEN("results \"a\"") {
        REQUIRE(std::string("a") ==
                parse(cons(letter, many(digit)), src));
      }
    }
  }
  GIVEN("an input: \"a123bc\"") {
    Source src = Source_new("a123bc");
    WHEN("apply cons(letter, many(digit))") {
      THEN("results \"a123\"") {
        REQUIRE(std::string("a123") ==
                parse(cons(letter, many(digit)), src));
      }
    }
  }
  GIVEN("an input: \"123\"") {
    Source src = Source_new("123");
    WHEN("apply cons(letter, many(digit))") {
      THEN("cause exception(\"not satisfy\")") {
        REQUIRE_THROWS_WITH(parse(cons(letter, many(digit)), src),
                            "not satisfy");
      }
    }
  }
  cparsec2_end();
}

SCENARIO("cons(p, ps)", "[cparsec2][parser][cons]") {
  cparsec2_init();
  GIVEN("an input: \"123 456 789\"") {
    Source src = Source_new("123 456 789");
    WHEN("apply cons(number, many(number))") {
      List(Int) xs = parse(cons(number, many(number)), src);
      THEN("results [123, 456, 789]") {
        int* itr = list_begin(xs);
        REQUIRE(123 == itr[0]);
        REQUIRE(456 == itr[1]);
        REQUIRE(789 == itr[2]);
      }
    }
    WHEN("apply cons(x, many(x)) where x = token(many1(digit))") {
      PARSER(String) x = token(many1(digit));
      List(String) xs = parse(cons(x, many(x)), src);
      THEN("results [\"123\", \"456\", \"789\"]") {
        const char** itr = list_begin(xs);
        REQUIRE(std::string("123") == itr[0]);
        REQUIRE(std::string("456") == itr[1]);
        REQUIRE(std::string("789") == itr[2]);
      }
    }
  }
  cparsec2_end();
}
