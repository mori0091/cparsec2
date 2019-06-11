/* -*- coding: utf-8-unix -*- */

#include <catch.hpp>
#include <cparsec2.hpp>

SCENARIO("crlf", "[cparsec2][parser][crlf]") {
  cparsec2_init();
  GIVEN("an input: \" \" (i.e. SPC)") {
    Source src = Source_new(" ");
    WHEN("apply crlf") {
      THEN("cause exception(\"expects '\\r' but was ' '\")") {
        REQUIRE_THROWS_WITH(parse(crlf, src), "expects '\r' but was ' '");
      }
    }
  }
  GIVEN("an input: \"\\t\" (i.e. TAB)") {
    Source src = Source_new("\t");
    WHEN("apply crlf") {
      THEN("cause exception(\"expects '\\r' but was '\\t'\")") {
        REQUIRE_THROWS_WITH(parse(crlf, src), "expects '\r' but was '\t'");
      }
    }
  }
  GIVEN("an input: \"\\r\" (i.e. CR)") {
    Source src = Source_new("\r");
    WHEN("apply crlf") {
      THEN("cause exception(\"too short\")") {
        REQUIRE_THROWS_WITH(parse(crlf, src), "too short");
      }
    }
  }
  GIVEN("an input: \"\\n\" (i.e. LF)") {
    Source src = Source_new("\n");
    WHEN("apply crlf") {
      THEN("cause exception(\"expects '\\r' but was '\\n'\")") {
        REQUIRE_THROWS_WITH(parse(crlf, src), "expects '\r' but was '\n'");
      }
    }
  }
  GIVEN("an input: \"\\r\\n\" (i.e. CR LF)") {
    Source src = Source_new("\r\n");
    WHEN("apply crlf") {
      THEN("results '\n')") {
        REQUIRE('\n' == parse(crlf, src));
      }
    }
  }
  GIVEN("an input: \"a\"") {
    Source src = Source_new("a");
    WHEN("apply crlf") {
      THEN("cause exception(\"expects '\\r' but was 'a'\")") {
        REQUIRE_THROWS_WITH(parse(crlf, src), "expects '\r' but was 'a'");
      }
    }
  }
  GIVEN("an input: \"\"") {
    Source src = Source_new("");
    WHEN("apply crlf") {
      THEN("cause exception(\"too short\")") {
        REQUIRE_THROWS_WITH(parse(crlf, src), "too short");
      }
    }
  }
  cparsec2_end();
}
