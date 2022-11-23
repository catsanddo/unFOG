char *subs(char *str)
{
    size_t length = strlen(str);

    while (str[--length - 1] != '/');

    return strndup(str, length);
}

char *FindFile(const char *pwd)
{
    const char f[] = ".unfog";
    char *path;
    char *result = 0;
    tinydir_dir dir;

    path = strdup(pwd);

    while (strcmp("/", path) != 0) {
        tinydir_open_sorted(&dir, path);
        for (int i = 0; i < dir.n_files; ++i) {
            tinydir_file file;
            tinydir_readfile_n(&dir, &file, i);

            if (file.is_reg && strcmp(file.name, f) == 0) {
                result = strdup(file.path);
                goto end;
            }
        }
        tinydir_close(&dir);

        char *tmp = path;
        path = subs(path);
        free(tmp);
    } end:
    free(path);

    return result;
}
