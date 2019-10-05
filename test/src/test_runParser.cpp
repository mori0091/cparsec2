/* -*- coding: utf-8-unix -*- */

#include <catch.hpp>
#include <cparsec2.hpp>

SCENARIO("runParser(p, input)", "[cparsec2][core][runParser]") {
  cparsec2_init();
  WHEN("r = runParser(anyChar, \"x\")") {
    auto r = runParser(anyChar, "x");
    THEN("consumed ok") {
      REQUIRE(r.succeeded);
      REQUIRE(1 == r.consumed);
      REQUIRE('x' == r.ok);
    }
  }
  WHEN("r = runParser(number, \"123\")") {
    auto r = runParser(number, "123");
    THEN("consumed ok") {
      REQUIRE(r.succeeded);
      REQUIRE(3 == r.consumed);
      REQUIRE(123 == r.ok);
    }
  }
  WHEN("r = runParser(anyChar, \"\")") {
    auto r = runParser(anyChar, "");
    THEN("empty error") {
      REQUIRE(!r.succeeded);
      REQUIRE(0 == r.consumed);
      REQUIRE(1 == r.err.pos.line);
      REQUIRE(1 == r.err.pos.column);
    }
  }
  WHEN("r = runParser(number, \"abc\")") {
    auto r = runParser(number, "abc");
    THEN("empty error") {
      REQUIRE(!r.succeeded);
      REQUIRE(0 == r.consumed);
      REQUIRE(1 == r.err.pos.line);
      REQUIRE(1 == r.err.pos.column);
    }
  }
  WHEN("r = runParser(seq(char1('a'), char1('b')), \"ac\")") {
    auto r = runParser(seq(char1('a'), char1('b')), "ac");
    THEN("consumed error") {
      REQUIRE(!r.succeeded);
      REQUIRE(1 == r.consumed);
      REQUIRE(1 == r.err.pos.line);
      REQUIRE(2 == r.err.pos.column);
    }
  }
  WHEN("r = runParser(\"ab\", \"ac\")") {
    auto r = runParser("ab", "ac");
    THEN("empty error") {
      REQUIRE(!r.succeeded);
      REQUIRE(0 == r.consumed);
      REQUIRE(1 == r.err.pos.line);
      REQUIRE(1 == r.err.pos.column);
    }
  }
  cparsec2_end();
}
