#include <assert.h>
#include <cparsec2/metac2.h>

#define STATIC_ASSERT(expr) static_assert((expr), "error: " #expr)

#define add(x, y) ((x) + (y))
#define mul(x, y) ((x) * (y))
#define mul10plusY(x, y) (10 * (x) + (y))

STATIC_ASSERT(IS_NIL());
STATIC_ASSERT(IS_NIL(a));
STATIC_ASSERT(!IS_NIL(a, b));
STATIC_ASSERT(!IS_NIL(, , ));
STATIC_ASSERT(!IS_NIL(, ));
STATIC_ASSERT(!IS_NIL(, a));
STATIC_ASSERT(!IS_NIL(a, ));
STATIC_ASSERT(!IS_NIL((a, b), (x, y), 3));

STATIC_ASSERT(IS_ENCLOSED(()));
STATIC_ASSERT(IS_ENCLOSED((a, b)));
STATIC_ASSERT(IS_ENCLOSED((a), b));
STATIC_ASSERT(!IS_ENCLOSED(a, b));
STATIC_ASSERT(!IS_ENCLOSED(a, (b)));

STATIC_ASSERT(IS_NULL());
STATIC_ASSERT(!IS_NULL(()));
STATIC_ASSERT(!IS_NULL(x));
STATIC_ASSERT(!IS_NULL((a, b)));
STATIC_ASSERT(!IS_NULL(1));

STATIC_ASSERT(IF_ENCLOSED((a, b))(10, 20) == 10);
STATIC_ASSERT(IF_ENCLOSED(a, b)(10, 20) == 20);

STATIC_ASSERT(FOLDL(add, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10) == 55);
STATIC_ASSERT(FOLDR(add, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10) == 55);

STATIC_ASSERT(FOLDL(mul10plusY, 1, 2) == 12);
STATIC_ASSERT(FOLDL(mul10plusY, 1, 2, 3) == 123);
STATIC_ASSERT(FOLDL(mul10plusY, 1, 2, 3, 4) == 1234);

STATIC_ASSERT(FOLDR(mul10plusY, 1, 2) == 12);
STATIC_ASSERT(FOLDR(mul10plusY, 1, 2, 3) == 33);
STATIC_ASSERT(FOLDR(mul10plusY, 1, 2, 3, 4) == 64);

STATIC_ASSERT(FOLDL(add, REVERSE(1, 2, 3, 4, 5, 6, 7, 8, 9, 10)) == 55);
STATIC_ASSERT(FOLDR(add, REVERSE(1, 2, 3, 4, 5, 6, 7, 8, 9, 10)) == 55);

STATIC_ASSERT(FOLDL(mul10plusY, REVERSE(1, 2)) == 21);
STATIC_ASSERT(FOLDL(mul10plusY, REVERSE(1, 2, 3)) == 321);
STATIC_ASSERT(FOLDL(mul10plusY, REVERSE(1, 2, 3, 4)) == 4321);

STATIC_ASSERT(FOLDR(mul10plusY, REVERSE(1, 2)) == 21);
STATIC_ASSERT(FOLDR(mul10plusY, REVERSE(1, 2, 3)) == 51);
STATIC_ASSERT(FOLDR(mul10plusY, REVERSE(1, 2, 3, 4)) == 91);

STATIC_ASSERT(FOLDL(STACK, (), (PUSH, 1), (PUSH, 2), (OP2, add)) == 3);
STATIC_ASSERT(FOLDL(STACK, (), (PUSH, 1), (PUSH, 2), (PUSH, 3), (PUSH, 4),
                    (OP2, add), (OP2, mul), (OP2, add)) == 15);

STATIC_ASSERT(TREE_FOLDL(add, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10) == 55);

STATIC_ASSERT(TREE_FOLDL(mul10plusY, 1, 2) == 12);
STATIC_ASSERT(TREE_FOLDL(mul10plusY, 1, 2, 3) == 123);
STATIC_ASSERT(TREE_FOLDL(mul10plusY, 1, 2, 3, 4) == 154);
STATIC_ASSERT(TREE_FOLDL(mul10plusY, 1, 2, 3, 4, 5) == 1545);

STATIC_ASSERT(TREE_FOLDL(mul10plusY, REVERSE(1, 2)) == 21);
STATIC_ASSERT(TREE_FOLDL(mul10plusY, REVERSE(1, 2, 3)) == 321);
STATIC_ASSERT(TREE_FOLDL(mul10plusY, REVERSE(1, 2, 3, 4)) == 451);
STATIC_ASSERT(TREE_FOLDL(mul10plusY, REVERSE(1, 2, 3, 4, 5)) == 5721);
