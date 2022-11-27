#ifndef ARENA_H
#define ARENA_H

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

Arena *ArenaAlloc(void);
void ArenaFree(Arena *arena);
void *ArenaPush(Arena *arena, size_t size);
void ArenaPop(Arena *arena, size_t size);

#endif
