#include <stddef.h>

#define StringVArg(s) s.length, s.str
#define StringLit(s) (String) { (s), sizeof(s)-1 }

typedef struct String String;
struct String {
    char *str;
    size_t length;
};

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
