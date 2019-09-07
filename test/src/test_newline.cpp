/* -*- coding: utf-8-unix -*- */

#include <catch.hpp>
#include <cparsec2.hpp>

SCENARIO("newline", "[cparsec2][parser][newline]") {
  cparsec2_init();
  GIVEN("an input: \" \" (i.e. SPC)") {
    Source src = Source_new(" ");
    WHEN("apply newline") {
      THEN("cause exception(\"expects <LF> but was ' '\")") {
        REQUIRE_THROWS_WITH(parse(newline, src),
                            "expects <LF> but was ' '");
      }
    }
  }
  GIVEN("an input: \"\\t\" (i.e. TAB)") {
    Source src = Source_new("\t");
    WHEN("apply newline") {
      THEN("cause exception(\"expects <LF> but was '\\t'\")") {
        REQUIRE_THROWS_WITH(parse(newline, src),
                            "expects <LF> but was '\\t'");
      }
    }
  }
  GIVEN("an input: \"\\r\" (i.e. CR)") {
    Source src = Source_new("\r");
    WHEN("apply newline") {
      THEN("cause exception(\"expects <LF> but was '\\r'\")") {
        REQUIRE_THROWS_WITH(parse(newline, src),
                            "expects <LF> but was '\\r'");
      }
    }
  }
  GIVEN("an input: \"\\n\" (i.e. LF)") {
    Source src = Source_new("\n");
    WHEN("apply newline") {
      THEN("results '\\n'") {
        REQUIRE('\n' == parse(newline, src));
      }
    }
  }
  GIVEN("an input: \"a\"") {
    Source src = Source_new("a");
    WHEN("apply newline") {
      THEN("cause exception(\"expects <LF> but was 'a'\")") {
        REQUIRE_THROWS_WITH(parse(newline, src),
                            "expects <LF> but was 'a'");
      }
    }
  }
  GIVEN("an input: \"\"") {
    Source src = Source_new("");
    WHEN("apply newline") {
      THEN("cause exception(\"too short\")") {
        REQUIRE_THROWS_WITH(parse(newline, src), "too short");
      }
    }
  }
  cparsec2_end();
}
