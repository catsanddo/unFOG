#ifndef STRING_H
#define STRING_H

#include <stddef.h>
#include "arena.h"

#define StringVArg(s) s.length, s.str
#define StringLit(s) (String) { (s), sizeof(s)-1 }

typedef struct String String;
struct String {
    char *str;
    size_t length;
};

String StringC(Arena *arena, const char *str);

#endif
