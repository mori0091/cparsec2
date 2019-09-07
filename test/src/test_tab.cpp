/* -*- coding: utf-8-unix -*- */

#include <catch.hpp>
#include <cparsec2.hpp>

SCENARIO("tab", "[cparsec2][parser][tab]") {
  cparsec2_init();
  GIVEN("an input: \" \" (i.e. SPC)") {
    Source src = Source_new(" ");
    WHEN("apply tab") {
      THEN("cause exception(\"expects a TAB but was ' '\")") {
        REQUIRE_THROWS_WITH(parse(tab, src), "expects a TAB but was ' '");
      }
    }
  }
  GIVEN("an input: \"\\t\" (i.e. TAB)") {
    Source src = Source_new("\t");
    WHEN("apply tab") {
      THEN("results '\\t'") {
        REQUIRE('\t' == parse(tab, src));
      }
    }
  }
  GIVEN("an input: \"\\r\" (i.e. CR)") {
    Source src = Source_new("\r");
    WHEN("apply tab") {
      THEN("cause exception(\"expects a TAB but was '\\r'\")") {
        REQUIRE_THROWS_WITH(parse(tab, src), "expects a TAB but was '\\r'");
      }
    }
  }
  GIVEN("an input: \"\\n\" (i.e. LF)") {
    Source src = Source_new("\n");
    WHEN("apply tab") {
      THEN("cause exception(\"expects a TAB but was '\\n'\")") {
        REQUIRE_THROWS_WITH(parse(tab, src), "expects a TAB but was '\\n'");
      }
    }
  }
  GIVEN("an input: \"a\"") {
    Source src = Source_new("a");
    WHEN("apply tab") {
      THEN("cause exception(\"expects a TAB but was 'a'\")") {
        REQUIRE_THROWS_WITH(parse(tab, src), "expects a TAB but was 'a'");
      }
    }
  }
  GIVEN("an input: \"\"") {
    Source src = Source_new("");
    WHEN("apply tab") {
      THEN("cause exception(\"too short\")") {
        REQUIRE_THROWS_WITH(parse(tab, src), "too short");
      }
    }
  }
  cparsec2_end();
}
