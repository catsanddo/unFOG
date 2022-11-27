#include <stdio.h>
#include <string.h>

#include <json.h>
#include <tinydir.h>

#include "path.c"
#include "arena.c"
#include "string.c"
#include "node.c"
#include "tags.c"

int main(int argc, char **argv)
{
    char *pwd = getenv("PWD");

    char *tag_file = FindFile(pwd);
    if (tag_file) {
        printf("%s\n", tag_file);
    } else {
        puts("No tag file found!");
        return 1;
    }

    Arena *node_arena = ArenaAlloc();
    Arena *json_arena = ArenaAlloc();
    NodePool pool = { .arena = node_arena };
    Node *node;
    {
        char *src = 0;
        size_t src_len = 0;
        FILE *file = fopen(tag_file, "r");
        if (!file) {
            return 1;
        }

        fseek(file, 0, SEEK_END);
        src_len = ftell(file);
        rewind(file);
        src = malloc(src_len+1);
        fread(src, 1, src_len, file);
        fclose(file);

        struct json_value_s *result = json_parse(src, src_len);
        free(src);
        node = NodesFromJson(&pool, result);
        free(result);
        ArenaClear(json_arena);
    }

    AddTag(&pool, &node, StringLit("foobar"));
    TagFile(&pool, &node, StringLit("foobar"), StringLit("TODO"));
    TagFile(&pool, &node, StringLit("foobar"), StringLit("TODO"));
    TagFile(&pool, &node, StringLit("code"), StringLit("TODO"));
    TagFile(&pool, &node, StringLit("f"), StringLit("TODO"));

    {
        struct json_value_s *result = JsonFromNodes(json_arena, node);
        String dump;
        dump.str = json_write_pretty(result, 0, 0, &dump.length);
        printf("%.*s\n", StringVArg(dump));
    }

    return 0;
}

/*
 * Usage:
 * Commands:
 *  ls <files...>
 *  tags
 *  tag <tag> [files...]
 *  rm <tag> [files...]
 */
