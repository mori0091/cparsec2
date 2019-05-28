/* -*- coding: utf-8-unix -*- */

#include <catch.hpp>
#include <cparsec2.hpp>
#include <unistd.h>

SCENARIO("parseTest(p, str)", "[cparsec2][core][parseTest]") {
  cparsec2_init();
  // CharParser
  WHEN("parseTest(digit, \"123\")") {
    THEN("results true") { REQUIRE(parseTest(digit, "123")); }
  }
  WHEN("parseTest(alpha, \"123\")") {
    THEN("results false") { REQUIRE_FALSE(parseTest(alpha, "123")); }
  }
  // StringParser
  WHEN("parseTest(many1(digit), \"123\")") {
    THEN("results true") { REQUIRE(parseTest(many1(digit), "123")); }
  }
  WHEN("parseTest(many1(alpha), \"123\")") {
    THEN("results false") {
      REQUIRE_FALSE(parseTest(many1(alpha), "123"));
    }
  }
  // TokenParser
  WHEN("parseTest(token(TK_NUMBER, many1(digit)), \"  123  \")") {
    THEN("results true") {
      REQUIRE(parseTest(token(TK_NUMBER, many1(digit)), "  123  "));
    }
  }
  WHEN("parseTest(token(TK_NUMBER, many1(alpha)), \"  123  \")") {
    THEN("results false") {
      REQUIRE_FALSE(parseTest(token(TK_NUMBER, many1(alpha)), "  123  "));
    }
  }
  cparsec2_end();
}

SCENARIO("PARSE_TEST(p, str)", "[cparsec2][core][PARSE_TEST]") {
  cparsec2_init();
  // CharParser
  WHEN("PARSE_TEST(digit, \"123\")") {
    THEN("results true") { REQUIRE(PARSE_TEST(digit, "123")); }
  }
  WHEN("PARSE_TEST(alpha, \"123\")") {
    THEN("results false") { REQUIRE_FALSE(PARSE_TEST(alpha, "123")); }
  }
  // StringParser
  WHEN("PARSE_TEST(many1(digit), \"123\")") {
    THEN("results true") { REQUIRE(PARSE_TEST(many1(digit), "123")); }
  }
  WHEN("PARSE_TEST(many1(alpha), \"123\")") {
    THEN("results false") {
      REQUIRE_FALSE(PARSE_TEST(many1(alpha), "123"));
    }
  }
  // TokenParser
  WHEN("PARSE_TEST(token(TK_NUMBER, many1(digit)), \"  123  \")") {
    THEN("results true") {
      REQUIRE(PARSE_TEST(token(TK_NUMBER, many1(digit)), "  123  "));
    }
  }
  WHEN("PARSE_TEST(token(TK_NUMBER, many1(alpha)), \"  123  \")") {
    THEN("results false") {
      REQUIRE_FALSE(PARSE_TEST(token(TK_NUMBER, many1(alpha)), "  123  "));
    }
  }
  cparsec2_end();
}
