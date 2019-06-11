/* -*- coding: utf-8-unix -*- */

#include <catch.hpp>
#include <cparsec2.hpp>

SCENARIO("space", "[cparsec2][parser][space]") {
  cparsec2_init();
  GIVEN("an input: \" \" (i.e. SPC)") {
    Source src = Source_new(" ");
    WHEN("apply space") {
      THEN("results ' '") {
        REQUIRE(' ' == parse(space, src));
      }
    }
  }
  GIVEN("an input: \"\\t\" (i.e. TAB)") {
    Source src = Source_new("\t");
    WHEN("apply space") {
      THEN("results '\\t'") {
        REQUIRE('\t' == parse(space, src));
      }
    }
  }
  GIVEN("an input: \"\\r\" (i.e. CR)") {
    Source src = Source_new("\r");
    WHEN("apply space") {
      THEN("results '\\r'") {
        REQUIRE('\r' == parse(space, src));
      }
    }
  }
  GIVEN("an input: \"\\n\" (i.e. LF)") {
    Source src = Source_new("\n");
    WHEN("apply space") {
      THEN("results '\\n'") {
        REQUIRE('\n' == parse(space, src));
      }
    }
  }
  GIVEN("an input: \"a\"") {
    Source src = Source_new("a");
    WHEN("apply space") {
      THEN("cause exception(\"not satisfy\")") {
        REQUIRE_THROWS_WITH(parse(space, src), "not satisfy");
      }
    }
  }
  GIVEN("an input: \"\"") {
    Source src = Source_new("");
    WHEN("apply space") {
      THEN("cause exception(\"too short\")") {
        REQUIRE_THROWS_WITH(parse(space, src), "too short");
      }
    }
  }
  cparsec2_end();
}
