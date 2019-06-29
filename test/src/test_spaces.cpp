/* -*- coding: utf-8-unix -*- */

#include <catch.hpp>
#include <cparsec2.hpp>

SCENARIO("spaces", "[cparsec2][parser][spaces]") {
  cparsec2_init();
  GIVEN("an input: \"   a\"") {
    Source src = Source_new("   a");
    WHEN("apply spaces") {
      THEN("results \"   \"") {
        REQUIRE(std::string("   ") == parse(spaces, src));
      }
    }
  }
  GIVEN("an input: \" \\t\\n\\r\" (i.e. SPC TAB LF CR)") {
    Source src = Source_new(" \t\n\r");
    WHEN("apply spaces") {
      THEN("results \" \\t\\n\\r\"") {
        REQUIRE(std::string(" \t\n\r") == parse(spaces, src));
      }
    }
  }
  GIVEN("an input: \"a\"") {
    Source src = Source_new("a");
    WHEN("apply spaces") {
      THEN("results \"\"") {
        REQUIRE(std::string("") == parse(spaces, src));
      }
    }
  }
  GIVEN("an input: \"\"") {
    Source src = Source_new("");
    WHEN("apply spaces") {
      THEN("results \"\"") {
        REQUIRE(std::string("") == parse(spaces, src));
      }
    }
  }
  cparsec2_end();
}
