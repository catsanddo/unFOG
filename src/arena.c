#include <stdint.h>
#include <stddef.h>
#include <sys/mman.h>

#define ARENA_MAX (16l << 20)
#define ARENA_INITIAL ( 4l << 10)

#define AlignPow2(n,a) ((n)+((a-1))&(~(a-1)))
#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) < (b) ? (a) : (b))
#define ArenaClear(a) ArenaPop(a, (a)->off)

typedef struct Arena Arena;
struct Arena {
    uint8_t *base;
    size_t off;
    size_t cap;
};

Arena *ArenaAlloc(void)
{
    Arena *result = 0;

    void *memory = mmap(0, ARENA_MAX, PROT_NONE,
            MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (memory != (void *) -1) {
        mprotect(memory, ARENA_INITIAL, PROT_READ | PROT_WRITE);
        result = (Arena *) memory;
        result->base = memory + 64;
        result->off = 0;
        result->cap = ARENA_INITIAL;
    }

    return result;
}

void ArenaFree(Arena *arena)
{
    if (!arena || !arena->base) {
        return;
    }
    arena->off = arena->cap = 0;
    munmap(arena->base, ARENA_MAX);
    arena->base = 0;
}

void *ArenaPush(Arena *arena, size_t size)
{
    unsigned char *result = 0;

    size = AlignPow2(size, 8);

    if (arena->off + size <= arena->cap) {
        result = arena->base + arena->off;
        arena->off += size;
    } else if (arena->off + size > ARENA_MAX) {
        return 0;
    } else {
        void *memory = (void *) arena->base - 64;
        arena->cap <<= 1;
        mprotect(memory, arena->cap, PROT_READ | PROT_WRITE);
        return ArenaPush(arena, size);
    }

    return result;
}

void ArenaPop(Arena *arena, size_t size)
{
    size = AlignPow2(size, 8);
    arena->off = max(arena->off - size, 0);
}
