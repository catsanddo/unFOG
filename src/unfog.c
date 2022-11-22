#include <stdio.h>
#include <dirent.h>

#include <md.h>
#include <md.c>
#include <tinydir.h>

char *FindFile(const char *pwd)
{
    const char f[] = ".unfog";
    char *path;
    tinydir_dir dir;

    tinydir_open(&dir, pwd);

    while (dir.has_next) {
        tinydir_file file;
        tinydir_readfile(&dir, &file);

        printf("%s\n", file.name);

        tinydir_next(&dir);
    }

    return path;
}

int main(int argc, char **argv)
{
    MD_Arena *arena = MD_ArenaAlloc();
    char *pwd = getenv("PWD");

    FindFile(pwd);

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
