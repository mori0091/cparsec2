/* -*- coding: utf-8-unix -*- */

#include <catch.hpp>
#include <cparsec2.hpp>

SCENARIO("endOfLine", "[cparsec2][parser][endOfLine]") {
  cparsec2_init();
  GIVEN("an input: \" \" (i.e. SPC)") {
    Source src = Source_new(" ");
    WHEN("apply endOfLine") {
      THEN("cause exception(\"expects <endOfLine> but was ' '\")") {
        REQUIRE_THROWS_WITH(parse(endOfLine, src),
                            "expects <endOfLine> but was ' '");
      }
    }
  }
  GIVEN("an input: \"\\t\" (i.e. TAB)") {
    Source src = Source_new("\t");
    WHEN("apply endOfLine") {
      THEN("cause exception(\"expects <endOfLine> but was '\\t'\")") {
        REQUIRE_THROWS_WITH(parse(endOfLine, src),
                            "expects <endOfLine> but was '\t'");
      }
    }
  }
  GIVEN("an input: \"\\r\" (i.e. CR)") {
    Source src = Source_new("\r");
    WHEN("apply endOfLine") {
      THEN("cause exception(\"too short\")") {
        REQUIRE_THROWS_WITH(parse(endOfLine, src), "too short");
      }
    }
  }
  GIVEN("an input: \"\\n\" (i.e. LF)") {
    Source src = Source_new("\n");
    WHEN("apply endOfLine") {
      THEN("results '\\n'") {
        REQUIRE('\n' == parse(endOfLine, src));
      }
    }
  }
  GIVEN("an input: \"\\r\\n\" (i.e. CR LF)") {
    Source src = Source_new("\r\n");
    WHEN("apply endOfLine") {
      THEN("results '\n')") {
        REQUIRE('\n' == parse(endOfLine, src));
      }
    }
  }
  GIVEN("an input: \"a\"") {
    Source src = Source_new("a");
    WHEN("apply endOfLine") {
      THEN("cause exception(\"expects <endOfLine> but was 'a'\")") {
        REQUIRE_THROWS_WITH(parse(endOfLine, src),
                            "expects <endOfLine> but was 'a'");
      }
    }
  }
  GIVEN("an input: \"\"") {
    Source src = Source_new("");
    WHEN("apply endOfLine") {
      THEN("cause exception(\"too short\")") {
        REQUIRE_THROWS_WITH(parse(endOfLine, src), "too short");
      }
    }
  }
  cparsec2_end();
}
