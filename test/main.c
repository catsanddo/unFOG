#include "utest.h"
#include "../include/json.h"
#include "../src/tags.h"
#include "../src/string.h"
#include "../src/arena.h"
#include "../src/node.h"
#include "../src/tags.c"
#include "../src/string.c"
#include "../src/arena.c"
#include "../src/node.c"

UTEST_MAIN();

// Is there a way to rewrite this in a way that doesn't crash
// if the test fails?
UTEST(arena, max_alloc)
{
    Arena *arena = ArenaAlloc();
    uint8_t *data = ArenaPush(arena, ARENA_MAX);

    // Touch all data
    for (size_t i = 0; i < ARENA_MAX; ++i) {
        data[i] = 0xDA;
    }

    ArenaFree(arena);
}

UTEST(node, push_and_pop)
{
    Arena *arena = ArenaAlloc();
    EXPECT_NE(arena, (void *) -1);

    Node *first = 0;
    for (intptr_t i = 0; i < 64; ++i) {
        Node *node = ArenaPush(arena, sizeof(Node));
        node->name.str = (void *) i;
        SLLPush(first, node);
    }

    for (intptr_t i = 63; i >= 0; --i) {
        Node *node;
        SLLPop(first, node);
        ASSERT_EQ(node->name.str, (void *) i);
    }

    ASSERT_FALSE(first);

    ArenaFree(arena);
}
