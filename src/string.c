#include "string.h"

String StringC(Arena *arena, const char *str)
{
    size_t length = 0;
    String result;

    while (str[length++] != 0);

    result.length = length - 1;
    result.str = ArenaPush(arena, length);
    for (size_t i = 0; i < length; ++i) {
        result.str[i] = str[i];
    }

    return result;
}
