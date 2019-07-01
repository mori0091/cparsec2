/* -*- coding: utf-8-unix -*- */

#include <catch.hpp>
#include <cparsec2.hpp>

SCENARIO("Source_new(FILE* fp)", "[cparsec2][parser][core][File]") {
  cparsec2_init();
  GIVEN("a file source whose contents is \"123 456 789\"") {
    char tmp[] = "tmpXXXXXX.txt";
    FILE* fp = fopen(tmp, "w+");
    REQUIRE(fp != nullptr);
    fprintf(fp, "123 456 789");
    fflush(fp);
    rewind(fp);
    Source src = Source_new(fp);
    WHEN("apply many(token(number))") {
      List(Int) xs = parse(many(token(number)), src);
      THEN("results [123, 456, 789]") {
        int* itr = list_begin(xs);
        REQUIRE(123 == itr[0]);
        REQUIRE(456 == itr[1]);
        REQUIRE(789 == itr[2]);
        AND_WHEN("apply anyChar") {
          THEN("causes an exception(\"too short\")") {
            REQUIRE_THROWS_WITH(parse(anyChar, src), "too short");
          }
        }
      }
    }
    if (fp) {
      fclose(fp);
      remove(tmp);
    }
  }
  cparsec2_end();
}
