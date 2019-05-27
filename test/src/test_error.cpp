/* -*- coding: utf-8-unix -*- */

#include <catch.hpp>
#include <cparsec2.hpp>

SCENARIO("construct an error message", "[cparsec2][core]") {
  GIVEN("fmt = \"expects '%c' but was '%c'\"") {
    const char* fmt = "expects '%c' but was '%c'";
    WHEN("error(fmt, 'c', 'b')") {
      THEN("results \"expects 'c' but was 'b'\"") {
        std::string actual( error(fmt, 'c', 'b') );
        REQUIRE("expects 'c' but was 'b'" == actual);
      }
    }
  }
}
