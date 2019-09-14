/* -*- coding: utf-8-unix -*- */

#include <catch.hpp>
#include <cparsec2.hpp>

SCENARIO("string1(str)", "[cparsec2][parser][string1]") {
  cparsec2_init();
  GIVEN("an input: \"a1234\"") {
    Source src = Source_new("a1234");
    WHEN("apply string1(\"a1234\")") {
      THEN("results \"a1234\"") {
        REQUIRE(std::string("a1234") == parse(string1("a1234"), src));
      }
    }
  }
  GIVEN("an input: \"a1234\"") {
    Source src = Source_new("a1234");
    WHEN("apply string1(\"a123\")") {
      THEN("results \"a123\"") {
        REQUIRE(std::string("a123") == parse(string1("a123"), src));
      }
    }
  }
  GIVEN("an input: \"a123bc\"") {
    Source src = Source_new("a123bc");
    WHEN("apply string1(\"a123\")") {
      THEN("results \"a123\"") {
        REQUIRE(std::string("a123") == parse(string1("a123"), src));
      }
    }
  }
  GIVEN("an input: \"a123bc\"") {
    Source src = Source_new("a123bc");
    WHEN("apply string1(\"a1234\")") {
      THEN("cause an error") {
        REQUIRE_THROWS(parse(string1("a1234"), src));
      }
    }
  }
  GIVEN("an input: \"a123\"") {
    Source src = Source_new("a123");
    WHEN("apply string1(\"a1234\")") {
      THEN("cause an error") {
        REQUIRE_THROWS(parse(string1("a1234"), src));
      }
    }
  }
  cparsec2_end();
}
