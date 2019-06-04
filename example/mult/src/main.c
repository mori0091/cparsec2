/* -*- coding: utf-8-unix -*- */

#include "mult.h"
#include <stdio.h>

int main(int argc, char** argv) {
  UNUSED(argc);
  UNUSED(argv);

  /* initialize CPARSEC2 library */
  cparsec2_init();

  Ctx ctx;
  TRY(&ctx) {
    /* input text is "100 200" */
    Source src = Source_new("100 200");
    /* parse the input text */
    int x = parse(mult(1), src, &ctx); /* x = 1 * 100 */
    parse(spaces, src, &ctx);          /* skip white-spaces */
    int y = parse(mult(2), src, &ctx); /* y = 2 * 200 */
    /* print x + y */
    printf("%d\n", x + y);
    return 0;
  }
  else {
    printf("error:%s\n", ctx.msg);
    return 1;
  }
}
