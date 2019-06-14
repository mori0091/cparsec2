#include "cparsec2.h"

const char* run_digit3(void* arg, Source src, Ctx* ex) {
  /* omit 'arg' since 'digit3' use no arguments */
  UNUSED(arg);

  Buff(Char) str = {0};
  Ctx ctx;
  TRY(&ctx) { /* try */
    for (int i = 0; i < 3; ++i) {
      buff_push(&str, parse(digit, src, &ctx));
    }
    return buff_finish(&str);
  }
  else { /* catch */
    mem_free(str.data);
    cthrow(ex, ctx.msg); /* re-throw */
  }
}

int main(int argc, char** argv) {
  UNUSED(argc);
  UNUSED(argv);

  /* NOTE: THIS MUST BE CALLED ONCE AT FIRST. */
  cparsec2_init();

  PARSER(String) digit3 = PARSER_GEN(String)(run_digit3, NULL);
  PARSE_TEST(digit3, "1234"); /* -> "123" */
  PARSE_TEST(digit3, "123");  /* -> "123" */
  PARSE_TEST(digit3, "12");   /* -> "error:too short" */
  PARSE_TEST(digit3, "a123"); /* -> "error:not satisfy" */

  return 0;
}
